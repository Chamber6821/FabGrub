//
// Created by Ivan on 27.01.2023.
//

#include "storage/AuthorityBasedStorage.h"
#include <doctest.h>
#include <functional>
#include <memory>
#include <random>
#include <ranges>
#include <set>
#include <utility>

using std::make_shared;
using std::make_unique;

using Id = int;
using File = file_management::File<Id>;
using FalseFile = file_management::FalseFile<Id>;
using Storage = file_management::Storage<Id>;
using ReadOnlyStorage = file_management::ReadOnlyStorage<Id>;
using AuthorityBasedStorage = file_management::AuthorityBasedStorageTests<Id>;

Id getRandomId() {
    static std::mt19937 generator(std::random_device{}());
    static std::uniform_int_distribution distribution(
        0, std::numeric_limits<int>::max());
    return distribution(generator);
}

class StubStorage : public Storage {
  public:
    using Container = std::set<const Id *>;
    using FileBuilder = std::function<std::unique_ptr<File>(
        StubStorage &parent, Container::const_iterator id)>;
    Container contained;
    FileBuilder fileBuilder;
    std::string pathPrefix;

    class StubFile : public File {
        Container &parentContainer;
        Container::const_iterator id;

      public:
        StubFile(Container &parentContainer, Container::const_iterator id)
            : parentContainer(parentContainer), id(id) {}

        bool exists() override { return id != parentContainer.end(); }

        bool cloneTo(Storage &storage) override {
            if (!exists())
                return false;
            auto &_storage = dynamic_cast<StubStorage &>(storage);
            _storage.put(**id);
            return true;
        }

        void remove() override {
            if (!exists())
                return;
            parentContainer.erase(id);
            id = parentContainer.end();
        }
    };

    StubStorage();

    explicit StubStorage(FileBuilder fileBuilder)
        : fileBuilder(std::move(fileBuilder)) {
        std::mt19937 generator(std::random_device{}());
        std::uniform_int_distribution distribution(
            0, std::numeric_limits<int>::max());

        pathPrefix = "some random prefix " +
                     std::to_string(distribution(generator)) +
                     " and concrete id ";
    }

    void put(const Id &id) { contained.insert(&id); }

    std::unique_ptr<File> findFor(const Id &id) override {
        auto it = contained.find(&id);
        if (it == contained.end())
            return make_unique<FalseFile>();
        return fileBuilder(*this, it);
    }

    std::filesystem::path pathFor(const Id &id) override {
        return pathPrefix + std::to_string(id);
    }

    void clear() override { contained.clear(); }
};

const StubStorage::FileBuilder createStubFile = [](auto &parent, auto id) {
    return make_unique<StubStorage::StubFile>(parent.contained, id);
};

const StubStorage::FileBuilder createFalseFile = [](...) {
    return make_unique<FalseFile>();
};

StubStorage::StubStorage() : StubStorage(createStubFile) {}

TEST_SUITE_BEGIN("AuthorityBasedStorage");

TEST_CASE("should proxy clear call to cache") {
    auto authority = new StubStorage();
    auto cache = new StubStorage();
    cache->put(123);
    cache->put(228);

    AuthorityBasedStorage testStorage(
        (std::shared_ptr<ReadOnlyStorage>(authority)),
        std::unique_ptr<Storage>(cache));

    testStorage.clear();

    REQUIRE(cache->contained.empty());
}

TEST_CASE("should proxy pathFor call to cache") {
    Id id = getRandomId();

    auto authority = new StubStorage();
    auto cache = new StubStorage();

    AuthorityBasedStorage testStorage(
        (std::shared_ptr<ReadOnlyStorage>(authority)),
        std::unique_ptr<Storage>(cache));

    REQUIRE_EQ(testStorage.pathFor(id), cache->pathFor(id));
}

TEST_CASE("should return not exists file") {
    Id id = getRandomId();

    AuthorityBasedStorage testStorage(make_shared<StubStorage>(),
                                      std::move(make_unique<StubStorage>()));

    auto file = testStorage.findFor(id);
    REQUIRE_NE(file, nullptr);
    REQUIRE_FALSE(file->exists());
}

TEST_CASE("should return file from cache if exists") {
    Id id = getRandomId();

    auto cache = new StubStorage();
    cache->put(id);

    AuthorityBasedStorage testStorage(make_shared<StubStorage>(),
                                      std::unique_ptr<Storage>(cache));

    REQUIRE(testStorage.findFor(id)->exists());
}

TEST_CASE("clone file to destination") {
    Id id = getRandomId();

    auto authority = new StubStorage();
    authority->put(id);

    auto cache = new StubStorage();

    auto destination = new StubStorage();

    AuthorityBasedStorage testStorage(
        (std::shared_ptr<ReadOnlyStorage>(authority)),
        std::unique_ptr<Storage>(cache));
    CHECK(testStorage.findFor(id)->cloneTo(*destination));

    SUBCASE("should save file to the inner storage") {
        REQUIRE((cache->findFor(id)->exists()));
    }
    SUBCASE("should save file to the destination") {
        REQUIRE((destination->findFor(id)->exists()));
    }
}

TEST_CASE("should return non-existent file if cloning from authority failed") {
    Id id = getRandomId();

    auto authority = new StubStorage(createFalseFile);
    authority->put(id);
    auto cache = new StubStorage();
    auto destination = new StubStorage();

    AuthorityBasedStorage testStorage(
        (std::shared_ptr<ReadOnlyStorage>(authority)),
        std::unique_ptr<Storage>(cache));

    REQUIRE_FALSE(testStorage.findFor(id)->cloneTo(*destination));
}

TEST_CASE("should return non-existent file if cloning from cache failed") {
    Id id = getRandomId();

    auto destination = new StubStorage();

    SUBCASE("if cloning from authority successful") {
        auto authority = new StubStorage();
        authority->put(id);
        auto cache = new StubStorage(createFalseFile);

        AuthorityBasedStorage testStorage(
            (std::shared_ptr<ReadOnlyStorage>(authority)),
            std::unique_ptr<Storage>(cache));

        REQUIRE_FALSE(testStorage.findFor(id)->cloneTo(*destination));
    }

    SUBCASE("if value already in cache") {
        auto authority = new StubStorage();
        auto cache = new StubStorage(createFalseFile);
        cache->put(id);

        AuthorityBasedStorage testStorage(
            (std::shared_ptr<ReadOnlyStorage>(authority)),
            std::unique_ptr<Storage>(cache));

        REQUIRE_FALSE(testStorage.findFor(id)->cloneTo(*destination));
    }
}

TEST_CASE("should clone file to cache if required") {
    Id id = getRandomId();

    auto authority = new StubStorage();
    authority->put(id);

    auto cache = new StubStorage();

    AuthorityBasedStorage testStorage(
        (std::shared_ptr<ReadOnlyStorage>(authority)),
        std::unique_ptr<Storage>(cache));

    CHECK(testStorage.requireFor(id));
    REQUIRE(cache->findFor(id)->exists());
}

TEST_SUITE_END();
