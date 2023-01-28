//
// Created by Ivan on 26.01.2023.
//

#pragma once

#include "../utils/Interface.h"
#include <memory>

template <class Id>
class Storage;

template <class Id>
class ReadOnlyStorage : public Interface {
  public:
    class ReadOnlyArchive : public Interface {
      public:
        virtual bool andCloneTo(Storage<Id> &storage) = 0;
    };

    virtual std::unique_ptr<ReadOnlyArchive> findReadOnlyFor(const Id &id) = 0;
};
