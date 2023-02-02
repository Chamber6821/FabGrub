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
    virtual bool exists() = 0;
    virtual bool cloneTo(Storage<Id> &storage) = 0;
};

template <class Id>
class File : public ReadOnlyFile<Id> {
  public:
    virtual void remove() = 0;
};

} // namespace file_management
