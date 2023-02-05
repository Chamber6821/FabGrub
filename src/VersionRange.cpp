//
// Created by Ivan on 05.02.2023.
//

#include "VersionRange.h"

const Version VersionRange::positive_infinity = {.major = std::numeric_limits<int>::max(),
                                                 .minor = std::numeric_limits<int>::max(),
                                                 .patch = std::numeric_limits<int>::max()};

const Version VersionRange::negative_infinity = {.major = 0, .minor = 0, .patch = 0};

VersionRange VersionRange::greaterThan(Version border) {
    auto left = border;
    left.patch++;
    return {left, positive_infinity};
}

VersionRange VersionRange::greaterOrEqualsThan(Version border) { return {border, positive_infinity}; }

VersionRange VersionRange::lessThan(Version border) {
    auto left = border;
    left.patch--;
    return {negative_infinity, left};
}

VersionRange VersionRange::lessOrEqualsThan(Version border) { return {negative_infinity, border}; }

VersionRange VersionRange::equals(Version target) { return {target, target}; }

VersionRange::VersionRange(Version from, Version to) : from(from), to(to) {}

bool VersionRange::in(Version version) const { return from <= version && version <= to; }

VersionRange VersionRange::operator&&(const VersionRange &other) const {
    return {std::max(from, other.from), std::min(to, other.to)};
}

bool VersionRange::empty() const { return to < from; }
