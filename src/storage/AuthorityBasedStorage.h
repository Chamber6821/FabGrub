//
// Created by Ivan on 26.01.2023.
//

#pragma once

#include "Storage.h"

namespace file_management {

template <class Id>
class AuthorityBasedStorage : public std::enable_shared_from_this<AuthorityBasedStorage<Id>>, public Storage<Id> {
    using Archive = Storage<Id>::Archive;
    using ReadOnlyArchive = Storage<Id>::ReadOnlyArchive;

    class RecommendedArchive : public Archive {
        std::shared_ptr<AuthorityBasedStorage> parent;
        std::unique_ptr<ReadOnlyArchive> source;

      public:
        bool andCloneTo(Storage<Id> &storage) override;
        bool andRemove() override;
    };

    std::shared_ptr<ReadOnlyStorage<Id>> authority;
    std::unique_ptr<Storage<Id>> storage;

  public:
    AuthorityBasedStorage(std::shared_ptr<ReadOnlyStorage<Id>> authority, std::unique_ptr<Storage<Id>> storage);

    std::unique_ptr<Archive> findFor(const Id &id) override;
    bool requireFor(const Id &id);
    void clear() override;

  protected:
    std::ostream rewriteFor(const Id &id) override;
};

} // namespace file_management
