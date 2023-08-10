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

namespace adapter {

class Version {
    int major, minor, patch;

  public:
    Version(int major, int minor, int patch)
        : major(major), minor(minor), patch(patch) {}

    Version(const ptr<::Version> &version)
        : Version(version->major(), version->minor(), version->patch()) {}

    auto operator<=>(const Version &other) const = default;

    friend auto &operator<<(std::ostream &out, const Version &v) noexcept {
        return out << v.major << '.' << v.minor << '.' << v.patch;
    }

    auto resurrect() const {
        std::stringstream ss;
        ss << (*this);
        return ss.str();
    }
};

static_assert(std::totally_ordered<Version>);

} // namespace adapter
