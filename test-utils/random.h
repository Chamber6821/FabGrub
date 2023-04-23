//
// Created by Ivan on 18.02.2023.
//

#pragma once

#include "contract/semver/Version.h"
#include "semver/MinVersionPart.h"
#include <string>

VersionPart randomVersionPart(VersionPart min = MinVersionPart,
                              VersionPart max = 100);

std::string getRandomNoSpaceName();
