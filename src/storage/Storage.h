//
// Created by Ivan on 26.01.2023.
//

#pragma once

#include "ReadOnlyStorage.h"
#include "File.h"
#include "forward.h"
#include <filesystem>

namespace file_management {

template <class Id>
class Storage : public ReadOnlyStorage<Id> {
  public:
    virtual std::unique_ptr<File<Id>> findFor(const Id &id) = 0;
    virtual std::filesystem::path pathFor(const Id &id) = 0;
    virtual void clear() = 0;

    inline std::unique_ptr<ReadOnlyFile<Id>> findReadOnlyFor(const Id &id) final;
};

template <class Id>
std::unique_ptr<ReadOnlyFile<Id>> Storage<Id>::findReadOnlyFor(const Id &id) {
    return findFor(id);
}

} // namespace file_management
