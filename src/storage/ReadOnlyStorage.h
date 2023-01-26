//
// Created by Ivan on 26.01.2023.
//

#pragma once

#include "../utils/Interface.h"
#include <memory>

class Storage;
class ModImplementation;

class ReadOnlyStorage : public Interface {
  public:
    class ReadOnlyArchive : public Interface {
      public:
        virtual bool andCloneTo(Storage &storage) = 0;
    };

    virtual std::unique_ptr<ReadOnlyArchive> findReadOnlyFor(const ModImplementation &mod) = 0;
};
