
//
// Created by Ivan on 19.04.2023.
//

#pragma once

#include "Version.h"
#include <concepts>

template <class T, class V>
concept VersionRange = Version<V> and requires(T &x, V &version) {
    { x.from() } -> std::derived_from<V>;
    { x.to() } -> std::derived_from<V>;
    { x.in(version) } -> std::convertible_to<bool>;
    { x.empty() } -> std::convertible_to<bool>;
};
