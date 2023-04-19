//
// Created by Ivan on 18.04.2023.
//

#pragma once

#include <concepts>

using VersionPart = int;

template <class T>
concept Version = requires(T &x) {
    { x.major() } -> std::same_as<VersionPart>;
    { x.minor() } -> std::same_as<VersionPart>;
    { x.patch() } -> std::same_as<VersionPart>;
};
