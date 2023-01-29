//
// Created by Ivan on 26.01.2023.
//

#pragma once

#include "ReadOnlyStorage.h"
#include <filesystem>

namespace file_management {

template <class Id>
class Storage : public ReadOnlyStorage<Id> {
  public:
    using ReadOnlyArchive = ReadOnlyStorage<Id>::ReadOnlyArchive;

    class Archive : public ReadOnlyArchive {
      public:
        virtual bool andRemove() = 0;
    };

    virtual std::unique_ptr<Archive> findFor(const Id &id) = 0;
    virtual void clear() = 0;

    std::unique_ptr<ReadOnlyArchive> findReadOnlyFor(const Id &id) override;

  protected:
    virtual std::shared_ptr<std::ostream> rewriteFor(const Id &id) = 0;
};

template <class Id>
std::unique_ptr<typename Storage<Id>::ReadOnlyArchive> Storage<Id>::findReadOnlyFor(const Id &id) {
    return findFor(id);
}

} // namespace file_management
