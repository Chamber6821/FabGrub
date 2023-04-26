//
// Created by Ivan on 18.02.2023.
//

#pragma once

#include "contract/semver/Version.h"
#include "semantic-type/Max.h"
#include "semantic-type/Min.h"
#include <string>

// calls randomVersionPart(min, max) with adequate values for log
auto randomVersionPart() -> VersionPart;

auto randomVersionPart(Min<VersionPart> min, Max<VersionPart> max)
    -> VersionPart;

// calls randomNoSpaceName(min, max) with adequate values for log
auto randomNoSpaceName() -> std::string;

auto randomNoSpaceName(Min<int> minLength, Max<int> maxLength) -> std::string;
