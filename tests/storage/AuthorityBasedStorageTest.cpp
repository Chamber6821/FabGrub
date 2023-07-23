//
// Created by Ivan on 27.01.2023.
//

#include "storage/AuthorityBasedStorage.h"
#include "doctest-nolint.h"
#include <functional>
#include <memory>
#include <random>
#include <set>
#include <utility>

using std::make_shared;
using std::make_unique;

using Id = int;
using File = file_management::File<Id>;
using FalseFile = file_management::FalseFile<Id>;
using Storage = file_management::Storage<Id>;
using AuthorityBasedStorage = file_management::AuthorityBasedStorageTests<Id>;

auto getRandomId() -> Id {
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

  private:
    Container _container;
    FileBuilder fileBuilder;
    std::string pathPrefix;

  public:
    class StubFile : public File {
        Container *parentContainer;
        Container::const_iterator id;

      public:
        // On Linux with Clang Container::const_iterator of the
        // trivially-copyable type
        StubFile(Container &parentContainer, Container::const_iterator id)
            : parentContainer(&parentContainer),
              id(std::move(id)) {} // NOLINT(*-move-const-arg)

        auto exists() -> bool override { return id != parentContainer->end(); }

        auto cloneTo(Storage &storage) -> bool override {
            if (!exists()) return false;
            auto &_storage = dynamic_cast<StubStorage &>(storage);
            _storage.put(**id);
            return true;
        }

        void remove() override {
            if (!exists()) return;
            parentContainer->erase(id);
            id = parentContainer->end();
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

    void put(const Id &id) { _container.insert(&id); }

    auto container() -> Container & { return _container; }

    auto findFor(const Id &id) -> std::unique_ptr<File> override {
        auto it = _container.find(&id);
        if (it == _container.end()) return make_unique<FalseFile>();
        return fileBuilder(*this, it);
    }

    auto pathFor(const Id &id) -> std::filesystem::path override {
        return pathPrefix + std::to_string(id);
    }

    void clear() override { _container.clear(); }
};

auto stubFileBuilder() -> StubStorage::FileBuilder {
    return [](auto &parent, auto id) {
        return make_unique<StubStorage::StubFile>(parent.container(), id);
    };
}

auto falseFileBuilder() -> StubStorage::FileBuilder {
    return [](auto &, auto) { return make_unique<FalseFile>(); };
}

StubStorage::StubStorage() : StubStorage(stubFileBuilder()) {}

TEST_SUITE_BEGIN("AuthorityBasedStorage");

TEST_CASE("should proxy clear call to cache") {
    auto authority = make_unique<StubStorage>();
    auto cache = make_unique<StubStorage>();

    auto id1 = getRandomId();
    auto id2 = getRandomId();

    cache->put(id1);
    cache->put(id2);

    AuthorityBasedStorage testStorage((std::move(authority)), std::move(cache));

    testStorage.clear();

    CHECK_FALSE(testStorage.findFor(id1)->exists());
    CHECK_FALSE(testStorage.findFor(id2)->exists());
}

TEST_CASE("should proxy pathFor call to cache") {
    auto id = getRandomId();

    auto authority = make_unique<StubStorage>();
    auto cache = make_unique<StubStorage>();
    auto expectedPath = cache->pathFor(id);

    AuthorityBasedStorage testStorage((std::move(authority)), std::move(cache));

    REQUIRE_EQ(testStorage.pathFor(id), expectedPath);
}

TEST_CASE("should return not exists file") {
    auto id = getRandomId();

    AuthorityBasedStorage testStorage(make_shared<StubStorage>(),
                                      std::move(make_unique<StubStorage>()));

    auto file = testStorage.findFor(id);
    REQUIRE_NE(file, nullptr);
    REQUIRE_FALSE(file->exists());
}

TEST_CASE("should return file from cache if exists") {
    auto id = getRandomId();

    auto cache = make_unique<StubStorage>();
    cache->put(id);

    AuthorityBasedStorage testStorage(make_shared<StubStorage>(),
                                      std::move(cache));

    REQUIRE(testStorage.findFor(id)->exists());
}

TEST_CASE("clone file to destination") {
    auto id = getRandomId();

    auto authority = make_shared<StubStorage>();
    authority->put(id);

    auto cache = make_unique<StubStorage>();

    auto destination = make_unique<StubStorage>();

    AuthorityBasedStorage testStorage((authority), std::move(cache));
    CHECK(testStorage.findFor(id)->cloneTo(*destination));

    SUBCASE("should save file to the inner storage") {
        authority->findFor(id)->remove();
        REQUIRE((testStorage.findFor(id)->exists()));
    }

    SUBCASE("should save file to the destination") {
        REQUIRE((destination->findFor(id)->exists()));
    }
}

TEST_CASE("should return non-existent file if cloning from authority failed") {
    auto id = getRandomId();

    auto authority = make_unique<StubStorage>(falseFileBuilder());
    authority->put(id);
    auto cache = make_unique<StubStorage>();
    auto destination = make_unique<StubStorage>();

    AuthorityBasedStorage testStorage((std::move(authority)), std::move(cache));

    REQUIRE_FALSE(testStorage.findFor(id)->cloneTo(*destination));
}

TEST_CASE("should return non-existent file if cloning from cache failed") {
    auto id = getRandomId();

    auto destination = make_unique<StubStorage>();

    SUBCASE("if cloning from authority successful") {
        auto authority = make_unique<StubStorage>();
        authority->put(id);
        auto cache = make_unique<StubStorage>(falseFileBuilder());

        AuthorityBasedStorage testStorage((std::move(authority)),
                                          std::move(cache));

        REQUIRE_FALSE(testStorage.findFor(id)->cloneTo(*destination));
    }

    SUBCASE("if value already in cache") {
        auto authority = make_unique<StubStorage>();
        auto cache = make_unique<StubStorage>(falseFileBuilder());
        cache->put(id);

        AuthorityBasedStorage testStorage((std::move(authority)),
                                          std::move(cache));

        REQUIRE_FALSE(testStorage.findFor(id)->cloneTo(*destination));
    }
}

TEST_CASE("should clone file to cache if required") {
    auto id = getRandomId();

    auto authority = make_shared<StubStorage>();
    authority->put(id);

    auto cache = make_unique<StubStorage>();

    AuthorityBasedStorage testStorage(authority, std::move(cache));

    CHECK(testStorage.requireFor(id));
    authority->findFor(id)->remove();
    REQUIRE(testStorage.findFor(id)->exists());
}

TEST_SUITE_END();
