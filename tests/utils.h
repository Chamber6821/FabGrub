//
// Created by Ivan on 18.02.2023.
//

#pragma once

#include "Version.h"

Version getRandomVersion();
Version getRandomVersionLessThan(Version barrier);
Version getRandomVersionGreaterThan(Version barrier);
Version getRandomVersionBetween(Version left, Version right);
