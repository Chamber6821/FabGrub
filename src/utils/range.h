//
// Created by Ivan on 08.08.2023.
//

#pragma once

#include "List.h"
#include "ptr.h"
#include <ranges>

template <class T>
auto to_range(ptr<List<T>> list) {
    return std::views::iota(list->count()) |
           std::views::transform([&](int index) { return list->at(index); });
}
