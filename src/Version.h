//
// Created by Ivan on 22.01.2023.
//

#pragma once

#include <compare>

struct Version {
    int major;
    int minor;
    int patch;

    auto operator<=>(const Version &other) const = default;
};
