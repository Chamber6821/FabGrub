//
// Created by Ivan on 26.01.2023.
//

#pragma once

#include "../utils/Interface.h"
#include <memory>

namespace file_management {

template <class Id>
class Storage;

template <class Id>
class ReadOnlyStorage : public Interface {
  public:
    class ReadOnlyFile : public Interface {
      public:
        virtual bool cloneTo(Storage<Id> &storage) = 0;
    };

    virtual std::unique_ptr<ReadOnlyFile> findReadOnlyFor(const Id &id) = 0;
};

} // namespace file_management
