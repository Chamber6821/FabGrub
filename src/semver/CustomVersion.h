//
// Created by Ivan on 22.01.2023.
//

#pragma once

#include "contract/semver/Version.h"
#include "utils/pure.h"
#include <compare>

struct CustomVersion final {
    const VersionPart _major, _minor, _patch;

  public:
    [[nodiscard]] pure major() const { return _major; }

    [[nodiscard]] pure minor() const { return _minor; }

    [[nodiscard]] pure patch() const { return _patch; }
};

static_assert(Version<CustomVersion>);
