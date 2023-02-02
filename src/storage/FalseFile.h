//
// Created by Ivan on 02.02.2023.
//

#pragma once

#include "File.h"

namespace file_management {

template <class Id>
class FalseFile : public File<Id> {
  public:
    bool exists() override { return false; }

    bool cloneTo(Storage<Id> &storage) override { return false; }

    void remove() override {}
};

} // namespace file_management
