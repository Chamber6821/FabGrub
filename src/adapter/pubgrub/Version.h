//
// Created by Ivan on 07.08.2023.
//

#pragma once

#include "semver/Version.h"
#include "utils/ptr.h"
#include <concepts>
#include <ostream>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>

namespace adapter {

class Version {
    ptr<::Version> version;

  public:
    explicit Version(ptr<::Version> version) : version(std::move(version)) {}

    [[nodiscard]] auto origin() const -> ptr<::Version> { return version; }

    auto operator==(const Version &other) const {
        return *version == *other.version;
    };

    auto operator<=>(const Version &other) const {
        return *version <=> *other.version;
    };
};

static_assert(std::totally_ordered<Version>);

} // namespace adapter
