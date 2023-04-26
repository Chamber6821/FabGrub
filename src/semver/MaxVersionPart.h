//
// Created by Ivan on 19.04.2023.
//

#pragma once

#include "contract/semver/Version.h"
#include "semantic-type/Max.h"
#include <limits>

constexpr auto MaxVersionPart = Max{std::numeric_limits<VersionPart>::max()};
