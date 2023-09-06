//
// Created by Ivan on 26.01.2023.
//

#pragma once

#include "FalseFile.h"
#include "Storage.h"

namespace file_management {

template <class TId>
class AuthorityBasedStorageTests : public Storage<TId> {
    class LazyFile : public File<TId> {
        std::shared_ptr<Storage<TId>> cache;
        std::unique_ptr<ReadOnlyFile<TId>> source;
        const TId *id;

      public:
        LazyFile(
            std::shared_ptr<Storage<TId>> cache,
            std::unique_ptr<ReadOnlyFile<TId>> source, const TId &id
        );
        auto exists() -> bool override;
        auto cloneTo(Storage<TId> &storage) -> bool override;
        void remove() override;
    };

    std::shared_ptr<ReadOnlyStorage<TId>> authority;
    std::shared_ptr<Storage<TId>> cache;

  public:
    AuthorityBasedStorageTests(
        std::shared_ptr<ReadOnlyStorage<TId>> authority,
        std::unique_ptr<Storage<TId>> storage
    );

    auto requireFor(const TId &id) -> bool;

    auto findFor(const TId &id) -> std::unique_ptr<File<TId>> override;
    auto pathFor(const TId &id) -> std::filesystem::path override;
    void clear() override;
};

template <class TId>
AuthorityBasedStorageTests<TId>::LazyFile::LazyFile(
    std::shared_ptr<Storage<TId>> cache,
    std::unique_ptr<ReadOnlyFile<TId>> source, const TId &id
)
    : cache(std::move(cache)), source(std::move(source)), id(&id) {}

template <class TId>
auto AuthorityBasedStorageTests<TId>::LazyFile::exists() -> bool {
    return false;
}

template <class TId>
auto AuthorityBasedStorageTests<TId>::LazyFile::cloneTo(Storage<TId> &storage)
    -> bool {
    source->cloneTo(*cache);
    return cache->findFor(*id)->cloneTo(storage);
}

template <class TId>
void AuthorityBasedStorageTests<TId>::LazyFile::remove() {}

template <class TId>
AuthorityBasedStorageTests<TId>::AuthorityBasedStorageTests(
    std::shared_ptr<ReadOnlyStorage<TId>> authority,
    std::unique_ptr<Storage<TId>> storage
)
    : authority(std::move(authority)), cache(std::move(storage)) {}

template <class TId>
auto AuthorityBasedStorageTests<TId>::requireFor(const TId &id) -> bool {
    authority->findReadOnlyFor(id)->cloneTo(*cache);
    return true;
}

template <class TId>
auto AuthorityBasedStorageTests<TId>::pathFor(const TId &id)
    -> std::filesystem::path {
    return cache->pathFor(id);
}

template <class TId>
void AuthorityBasedStorageTests<TId>::clear() {
    cache->clear();
}

template <class TId>
auto AuthorityBasedStorageTests<TId>::findFor(const TId &id)
    -> std::unique_ptr<File<TId>> {
    if (auto cached = cache->findFor(id); cached->exists()) return cached;

    if (auto recommended = authority->findReadOnlyFor(id);
        recommended->exists())
        return std::make_unique<LazyFile>(cache, std::move(recommended), id);

    return std::make_unique<FalseFile<TId>>();
}

} // namespace file_management
