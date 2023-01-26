//
// Created by Ivan on 26.01.2023.
//

#pragma once

#include "Storage.h"

class AuthorityBasedStorage : public std::enable_shared_from_this<AuthorityBasedStorage>, public Storage {
    class RecommendedArchive : public Archive {
        std::shared_ptr<AuthorityBasedStorage> parent;
        std::unique_ptr<ReadOnlyArchive> source;

      public:
        bool andCloneTo(Storage &storage) override;
        bool andRemove() override;
    };

    std::shared_ptr<ReadOnlyStorage> authority;
    std::unique_ptr<Storage> storage;

  public:
    AuthorityBasedStorage(std::shared_ptr<ReadOnlyStorage> authority, std::unique_ptr<Storage> storage);

    bool requireFor(const ModImplementation &mod);

    std::unique_ptr<Archive> findFor(const ModImplementation &mod) override;
    std::filesystem::path folder() override;
    void clear() override;
};
