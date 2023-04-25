//
// Created by Ivan on 26.01.2023.
//

#pragma once

#include "File.h"
#include "ReadOnlyStorage.h"
#include "forward.h"
#include <filesystem>

namespace file_management {

template <class TId>
class Storage : public ReadOnlyStorage<TId> {
  public:
    virtual auto findFor(const TId &id) -> std::unique_ptr<File<TId>> = 0;
    virtual auto pathFor(const TId &id) -> std::filesystem::path = 0;
    virtual void clear() = 0;

    inline auto findReadOnlyFor(const TId &id)
        -> std::unique_ptr<ReadOnlyFile<TId>> final {
        return findFor(id);
    }
};

} // namespace file_management
