//
// Created by Ivan on 18.02.2023.
//

#pragma once

#include "semver/Version.h"
#include "semver/VersionRange.h"

Version getRandomVersion();
Version getRandomVersionLessThan(Version barrier);
Version getRandomVersionGreaterThan(Version barrier);
Version getRandomVersionBetween(Version left, Version right);

VersionRange getRandomRange();
VersionRange getRandomNotEmptyRange();
VersionRange getRandomEmptyRange();

std::string getRandomNoSpaceName();
