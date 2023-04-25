//
// Created by Ivan on 22.01.2023.
//

#pragma once

#include "contract/semver/Version.h"
#include "utils/pure.h"
#include <compare>

class CustomVersion final {
    VersionPart _major, _minor, _patch;

  public:
    // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
    CustomVersion(VersionPart major, VersionPart minor, VersionPart patch)
        : _major(major), _minor(minor), _patch(patch) {}

    [[nodiscard]] pure major() const { return _major; }

    [[nodiscard]] pure minor() const { return _minor; }

    [[nodiscard]] pure patch() const { return _patch; }
};

static_assert(Version<CustomVersion>);
