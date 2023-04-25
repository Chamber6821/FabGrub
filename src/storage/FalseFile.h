//
// Created by Ivan on 02.02.2023.
//

#pragma once

#include "File.h"

namespace file_management {

template <class TId>
class FalseFile : public File<TId> {
  public:
    auto exists() -> bool override { return false; }

    auto cloneTo(Storage<TId> & /*unused*/) -> bool override { return false; }

    void remove() override {}
};

} // namespace file_management
