//
// Created by Ivan on 08.08.2023.
//

#pragma once

#include "Version.h"
#include "utils/ptr.h"
#include <limits>
#include <stdexcept>
#include <utility>

class NextPatch : public Version {
    ptr<Version> version;
    int min, max;

  public:
    NextPatch(ptr<Version> version, int min, int max)
        : version(std::move(version)), min(min), max(max) {}

    explicit NextPatch(ptr<Version> version)
        : NextPatch(std::move(version), 0, std::numeric_limits<int>::max()) {}

    [[nodiscard]] auto major() const -> int override {
        if (version->patch() != max) return version->major();
        if (version->minor() != max) return version->major();
        if (version->major() != max) return version->major() + 1;

        throw std::overflow_error("Version overflowed");
    }

    [[nodiscard]] auto minor() const -> int override {
        if (version->patch() != max) return version->minor();
        if (version->minor() != max) return version->minor() + 1;
        if (version->major() != max) return min;

        throw std::overflow_error("Version overflowed");
    }

    [[nodiscard]] auto patch() const -> int override {
        if (version->patch() != max) return version->patch() + 1;
        if (version->minor() != max) return min;
        if (version->major() != max) return min;

        throw std::overflow_error("Version overflowed");
    }
};
