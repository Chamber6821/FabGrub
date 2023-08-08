//
// Created by Ivan on 08.08.2023.
//

#pragma once

#include "List.h"
#include <vector>

template <class T>
class MemList : public List<T> {
    std::vector<T> reqs;

  public:
    explicit MemList(std::vector<T> reqs)
        : reqs(std::move(reqs)) {}

    MemList(std::initializer_list<T> reqs)
        : MemList(std::vector(reqs)) {}

    template <std::convertible_to<T>... Args>
    explicit MemList(Args... args)
        : MemList({args...}) {}

    auto count() -> int override { return static_cast<int>(reqs.size()); };

    auto at(int index) -> T override { return reqs.at(index); };
};
