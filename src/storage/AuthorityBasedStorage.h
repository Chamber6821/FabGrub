//
// Created by Ivan on 26.01.2023.
//

#pragma once

#include "Storage.h"

namespace file_management {

template <class Id>
class AuthorityBasedStorage : public Storage<Id> {
    using File = Storage<Id>::File;
    using ReadOnlyFile = Storage<Id>::ReadOnlyFile;

    class RecommendedArchive : public File {
        std::shared_ptr<Storage<Id>> cache;
        std::unique_ptr<ReadOnlyFile> source;
        const Id &id;

      public:
        RecommendedArchive(std::shared_ptr<Storage<Id>> cache, std::unique_ptr<ReadOnlyFile> source, const Id &id);

        bool cloneTo(Storage<Id> &destination) override;
        bool remove() override;
    };

    std::shared_ptr<ReadOnlyStorage<Id>> authority;
    std::shared_ptr<Storage<Id>> cache;

  public:
    AuthorityBasedStorage(std::shared_ptr<ReadOnlyStorage<Id>> authority, std::unique_ptr<Storage<Id>> storage);

    bool requireFor(const Id &id);

    std::unique_ptr<File> findFor(const Id &id) override;
    std::filesystem::path pathFor(const Id &id) override;
    void clear() override;
};

} // namespace file_management
