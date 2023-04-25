//
// Created by Ivan on 19.04.2023.
//

#pragma once

#include "MaxVersionPart.h"
#include "contract/semver/Version.h"
#include "utils/pure.h"

class MonotonousVersion final {
    VersionPart part;

  public:
    constexpr explicit MonotonousVersion(VersionPart part) : part(part) {}

    [[nodiscard]] pure major() const { return part; }

    [[nodiscard]] pure minor() const { return part; }

    [[nodiscard]] pure patch() const { return part; }
};

static_assert(Version<MonotonousVersion>);
