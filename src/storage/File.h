//
// Created by Ivan on 02.02.2023.
//

#pragma once

#include "forward.h"
#include "utils/Interface.h"
#include <memory>

namespace file_management {

template <class Id>
class ReadOnlyFile : public Interface {
  public:
    virtual auto exists() -> bool = 0;
    virtual auto cloneTo(Storage<Id> &storage) -> bool = 0;
};

template <class TId>
class File : public ReadOnlyFile<TId> {
  public:
    virtual void remove() = 0;
};

} // namespace file_management
