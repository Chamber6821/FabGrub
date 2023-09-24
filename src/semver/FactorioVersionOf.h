#pragma once

#include "MajorOf.h"
#include "MinorOf.h"
#include "OptionalPatch.h"
#include "VersionFromParts.h"

class FactorioVersionOf : public VersionFromParts {
  public:
    explicit FactorioVersionOf(const std::string &version)
        : VersionFromParts(
              make<MajorOf>(version), make<MinorOf>(version),
              make<OptionalPatch>(version, 0)
          ) {}
};
