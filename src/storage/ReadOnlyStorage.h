//
// Created by Ivan on 26.01.2023.
//

#pragma once

#include "forward.h"
#include "utils/Interface.h"
#include <memory>

namespace file_management {

template <class Id>
class ReadOnlyStorage : public Interface {
  public:
    virtual auto findReadOnlyFor(const Id &id)
        -> std::unique_ptr<ReadOnlyFile<Id>> = 0;
};

} // namespace file_management
