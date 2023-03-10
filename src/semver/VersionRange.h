//
// Created by Ivan on 05.02.2023.
//

#pragma once

#include "Version.h"
#include <limits>
#include <ostream>

class VersionRange {
    Version _from;
    Version _to;

  public:
    static const Version positive_infinity;
    static const Version negative_infinity;
    static const VersionRange nothing;
    static const VersionRange all;

    static VersionRange greaterThan(Version border);
    static VersionRange greaterOrEqualsThan(Version border);

    static VersionRange lessThan(Version border);
    static VersionRange lessOrEqualsThan(Version border);

    static VersionRange equals(Version target);

    VersionRange(Version from, Version to);

    [[nodiscard]] Version from() const;
    [[nodiscard]] Version to() const;

    [[nodiscard]] bool in(const Version &version) const;
    [[nodiscard]] bool empty() const;

    [[nodiscard]] VersionRange wideUnion(const VersionRange &range) const;

    VersionRange operator&&(const VersionRange &other) const;
    bool operator==(const VersionRange &other) const = default;
};