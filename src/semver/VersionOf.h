//
// Created by Ivan on 18.04.2023.
//

#pragma once

#include "MajorOf.h"
#include "MinorOf.h"
#include "PatchOf.h"
#include "VersionFromParts.h"
#include "utils/parseInt.h"
#include <algorithm>

// Parse the string with the version looks like '1.0.0' (without spaces)
class VersionOf : public VersionFromParts {
  public:
    explicit VersionOf(const std::string &version)
        : VersionFromParts(
              make<MajorOf>(version), make<MinorOf>(version),
              make<PatchOf>(version)
          ) {}
};
