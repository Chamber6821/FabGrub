//
// Created by Ivan on 26.01.2023.
//

#pragma once

#include "FalseFile.h"
#include "Storage.h"

namespace file_management {

template <class Id>
class AuthorityBasedStorageTests : public Storage<Id> {
    class LazyFile : public File<Id> {
        std::shared_ptr<Storage<Id>> cache;
        std::unique_ptr<ReadOnlyFile<Id>> source;
        const Id &id;

      public:
        LazyFile(std::shared_ptr<Storage<Id>> cache,
                 std::unique_ptr<ReadOnlyFile<Id>> source, const Id &id);
        bool exists() override;
        bool cloneTo(Storage<Id> &storage) override;
        void remove() override;
    };

    std::shared_ptr<ReadOnlyStorage<Id>> authority;
    std::shared_ptr<Storage<Id>> cache;

  public:
    AuthorityBasedStorageTests(std::shared_ptr<ReadOnlyStorage<Id>> authority,
                               std::unique_ptr<Storage<Id>> storage);

    bool requireFor(const Id &id);

    std::unique_ptr<File<Id>> findFor(const Id &id) override;
    std::filesystem::path pathFor(const Id &id) override;
    void clear() override;
};

template <class Id>
AuthorityBasedStorageTests<Id>::LazyFile::LazyFile(
    std::shared_ptr<Storage<Id>> cache,
    std::unique_ptr<ReadOnlyFile<Id>> source, const Id &id)
    : cache(cache), source(std::move(source)), id(id) {}

template <class Id>
bool file_management::AuthorityBasedStorageTests<Id>::LazyFile::exists() {
    return false;
}

template <class Id>
bool file_management::AuthorityBasedStorageTests<Id>::LazyFile::cloneTo(
    Storage<Id> &storage) {
    source->cloneTo(*cache);
    return cache->findFor(id)->cloneTo(storage);
}

template <class Id>
void file_management::AuthorityBasedStorageTests<Id>::LazyFile::remove() {}

template <class Id>
AuthorityBasedStorageTests<Id>::AuthorityBasedStorageTests(
    std::shared_ptr<ReadOnlyStorage<Id>> authority,
    std::unique_ptr<Storage<Id>> storage)
    : authority(authority), cache(std::move(storage)) {}

template <class Id>
bool AuthorityBasedStorageTests<Id>::requireFor(const Id &id) {
    authority->findReadOnlyFor(id)->cloneTo(*cache);
    return true;
}

template <class Id>
std::filesystem::path AuthorityBasedStorageTests<Id>::pathFor(const Id &id) {
    return cache->pathFor(id);
}

template <class Id>
void AuthorityBasedStorageTests<Id>::clear() {
    cache->clear();
}

template <class Id>
std::unique_ptr<File<Id>>
AuthorityBasedStorageTests<Id>::findFor(const Id &id) {
    if (auto cached = cache->findFor(id); cached->exists())
        return cached;

    if (auto recommended = authority->findReadOnlyFor(id);
        recommended->exists())
        return std::make_unique<LazyFile>(cache, std::move(recommended), id);

    return std::make_unique<FalseFile<Id>>();
}

} // namespace file_management
