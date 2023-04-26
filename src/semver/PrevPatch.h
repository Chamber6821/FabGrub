//
// Created by Ivan on 19.04.2023.
//

#pragma once

#include "MaxVersionPart.h"
#include "MinVersionPart.h"
#include "contract/semver/Version.h"
#include "utils/pure.h"
#include <stdexcept>

template <Version T>
class PrevPatch final {
    T version;
    Min<VersionPart> min;
    Max<VersionPart> max;

  public:
    constexpr PrevPatch(T version, Min<VersionPart> min, Max<VersionPart> max)
        : version(version), min(min), max(max) {
        static_assert(Version<PrevPatch>);
    }

    constexpr explicit PrevPatch(T version)
        : PrevPatch(version, MinVersionPart, MaxVersionPart) {}

    [[nodiscard]] pure major() const {
        if (version.patch() != min.value()) return version.major();
        if (version.minor() != min.value()) return version.major();
        if (version.major() != min.value()) return version.major() - 1;

        throw std::underflow_error("Version underflowed");
    }

    [[nodiscard]] pure minor() const {
        if (version.patch() != min.value()) return version.minor();
        if (version.minor() != min.value()) return version.minor() - 1;
        if (version.major() != min.value()) return max.value();

        throw std::underflow_error("Version underflowed");
    }

    [[nodiscard]] pure patch() const {
        if (version.patch() != min.value()) return version.patch() - 1;
        if (version.minor() != min.value()) return max.value();
        if (version.major() != min.value()) return max.value();

        throw std::underflow_error("Version underflowed");
    }
};
