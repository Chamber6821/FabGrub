//
// Created by Ivan on 18.02.2023.
//

#pragma once

#include "contract/semver/Version.h"
#include "semantic-type/Max.h"
#include "semantic-type/Min.h"
#include <random>
#include <string>

template <std::integral T>
auto random(Min<T> min, Max<T> max) -> T {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution des{min.value(), max.value()};
    return des(gen);
}

// calls randomVersionPart(min, max) with adequate values for log
auto randomVersionPart() -> VersionPart;

auto randomVersionPart(Min<VersionPart> min, Max<VersionPart> max)
    -> VersionPart;

// calls randomNoSpaceName(min, max) with adequate values for log
auto randomNoSpaceName() -> std::string;

auto randomNoSpaceName(Min<int> minLength, Max<int> maxLength) -> std::string;
