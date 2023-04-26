//
// Created by Ivan on 19.04.2023.
//

#pragma once

#include "MaxVersionPart.h"
#include "MinVersionPart.h"
#include "contract/semver/Version.h"
#include "semantic-type/Max.h"
#include "semantic-type/Min.h"
#include "utils/pure.h"
#include <stdexcept>

template <Version T>
class NextPatch final {
    T version;
    Min<VersionPart> min;
    Max<VersionPart> max;

  public:
    constexpr NextPatch(T version, Min<VersionPart> min, Max<VersionPart> max)
        : version(version), min(min), max(max) {
        static_assert(Version<NextPatch>);
    }

    constexpr explicit NextPatch(T version)
        : NextPatch(version, Min(MinVersionPart), Max(MaxVersionPart)) {}

    [[nodiscard]] pure major() const {
        if (version.patch() != max.value()) return version.major();
        if (version.minor() != max.value()) return version.major();
        if (version.major() != max.value()) return version.major() + 1;

        throw std::overflow_error("Version overflowed");
    }

    [[nodiscard]] pure minor() const {
        if (version.patch() != max.value()) return version.minor();
        if (version.minor() != max.value()) return version.minor() + 1;
        if (version.major() != max.value()) return min.value();

        throw std::overflow_error("Version overflowed");
    }

    [[nodiscard]] pure patch() const {
        if (version.patch() != max.value()) return version.patch() + 1;
        if (version.minor() != max.value()) return min.value();
        if (version.major() != max.value()) return min.value();

        throw std::overflow_error("Version overflowed");
    }
};
