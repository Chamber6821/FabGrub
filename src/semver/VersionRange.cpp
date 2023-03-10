//
// Created by Ivan on 05.02.2023.
//

#include "VersionRange.h"

const Version VersionRange::positive_infinity = {
    .major = std::numeric_limits<int>::max(),
    .minor = std::numeric_limits<int>::max(),
    .patch = std::numeric_limits<int>::max()};

const Version VersionRange::negative_infinity = {
    .major = 0, .minor = 0, .patch = 0};

const VersionRange VersionRange::nothing = {VersionRange::positive_infinity,
                                            VersionRange::negative_infinity};

const VersionRange VersionRange::all = {VersionRange::negative_infinity,
                                        VersionRange::positive_infinity};

VersionRange VersionRange::greaterThan(Version border) {
    auto left = border;
    left.patch++;
    return {left, positive_infinity};
}

VersionRange VersionRange::greaterOrEqualsThan(Version border) {
    return {border, positive_infinity};
}

VersionRange VersionRange::lessThan(Version border) {
    auto left = border;
    left.patch--;
    return {negative_infinity, left};
}

VersionRange VersionRange::lessOrEqualsThan(Version border) {
    return {negative_infinity, border};
}

VersionRange VersionRange::equals(Version target) { return {target, target}; }

VersionRange::VersionRange(Version from, Version to) : _from(from), _to(to) {
    if (empty())
        *this = nothing;
}

bool VersionRange::in(const Version &version) const {
    return _from <= version && version <= _to;
}

VersionRange VersionRange::operator&&(const VersionRange &other) const {
    return {std::max(_from, other._from), std::min(_to, other._to)};
}

bool VersionRange::empty() const { return _to < _from; }

VersionRange VersionRange::wideUnion(const VersionRange &range) const {
    return {std::min(_from, range._from), std::max(_to, range._to)};
}

Version VersionRange::from() const { return _from; }

Version VersionRange::to() const { return _to; }
