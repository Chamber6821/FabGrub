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
    using ReadOnlyFile = ReadOnlyStorage<Id>::ReadOnlyFile;

    class File : public ReadOnlyFile {
      public:
        virtual bool remove() = 0;
    };

    virtual std::unique_ptr<File> findFor(const Id &id) = 0;
    virtual std::filesystem::path pathFor(const Id &id) = 0;
    virtual void clear() = 0;

    std::unique_ptr<ReadOnlyFile> findReadOnlyFor(const Id &id) override;
};

template <class Id>
std::unique_ptr<typename Storage<Id>::ReadOnlyFile> Storage<Id>::findReadOnlyFor(const Id &id) {
    return findFor(id);
}

} // namespace file_management
