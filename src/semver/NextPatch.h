//
// Created by Ivan on 08.08.2023.
//

#pragma once

#include "Version.h"
#include "utils/ptr.h"
#include <limits>
#include <stdexcept>

class NextPatch : public Version {
    ptr<Version> version;
    int min, max;

  public:
    NextPatch(ptr<Version> version, int min, int max)
        : version(version), min(min), max(max) {}

    NextPatch(ptr<Version> version)
        : NextPatch(version, 0, std::numeric_limits<int>::max()) {}

    auto major() -> int override {
        if (version->patch() != max) return version->major();
        if (version->minor() != max) return version->major();
        if (version->major() != max) return version->major() + 1;

        throw std::overflow_error("Version overflowed");
    }

    auto minor() -> int override {
        if (version->patch() != max) return version->minor();
        if (version->minor() != max) return version->minor() + 1;
        if (version->major() != max) return min;

        throw std::overflow_error("Version overflowed");
    }

    auto patch() -> int override {
        if (version->patch() != max) return version->patch() + 1;
        if (version->minor() != max) return min;
        if (version->major() != max) return min;

        throw std::overflow_error("Version overflowed");
    }
};
