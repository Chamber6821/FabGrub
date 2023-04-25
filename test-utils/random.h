//
// Created by Ivan on 18.02.2023.
//

#pragma once

#include "contract/semver/Version.h"
#include <string>

// calls randomVersionPart(min, max) with adequate values for log
auto randomVersionPart() -> VersionPart;

auto randomVersionPart(VersionPart min, VersionPart max) -> VersionPart;

// calls randomNoSpaceName(min, max) with adequate values for log
auto randomNoSpaceName() -> std::string;

auto randomNoSpaceName(int minLength, int maxLength) -> std::string;
