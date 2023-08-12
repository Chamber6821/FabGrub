//
// Created by Ivan on 08.08.2023.
//

#pragma once

#include "List.h"
#include "ptr.h"
#include <ranges>

template <class T>
auto to_range(ptr<List<T>> list) {
    return std::views::iota(0, list->count()) |
           std::views::transform([list](int index) { return list->at(index); });
}

namespace details {
template <class T>
struct constructor_of {
    template <class... Args>
    constexpr auto operator()(Args &&...args) const -> T {
        return T(std::forward<Args>(args)...);
    }
};
} // namespace details

template <class T>
constexpr details::constructor_of<T> constructor;
