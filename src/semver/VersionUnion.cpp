//
// Created by Ivan on 20.02.2023.
//

#include "VersionUnion.h"
#include <algorithm>
#include <ranges>

template <std::ranges::range L, std::ranges::range R>
static auto cartesian_product(L left, R right) {
    auto combineWithRight = [&](auto &x) {
        auto makePair = [&](auto &y) { return std::make_pair(x, y); };
        return right | std::views::transform(makePair);
    };

    return left                                      //
           | std::views::transform(combineWithRight) //
           | std::views::join;
}

VersionUnion VersionUnion::nothing() {
    return VersionUnion(VersionRange::nothing);
}

VersionUnion::VersionUnion(const VersionRange &range) {
    if (!range.empty())
        ranges.push_back(range);
}

VersionUnion VersionUnion::operator||(const VersionUnion &other) const {
    if (ranges.empty() && other.ranges.empty())
        return VersionUnion::nothing();

    std::vector<VersionRange> dump;
    dump.reserve(ranges.size() + other.ranges.size());
    std::ranges::copy(ranges, back_inserter(dump));
    std::ranges::copy(other.ranges, back_inserter(dump));

    std::ranges::sort(dump, {}, &VersionRange::from);

    std::vector<VersionRange> result;
    auto accumulator = dump.front();
    for (auto &x : dump | std::views::drop(1)) {
        if ((accumulator && x).empty()) {
            result.push_back(accumulator);
            accumulator = x;
        }
        accumulator = accumulator.wideUnion(x);
    }
    result.push_back(accumulator);

    return VersionUnion(std::move(result));
}

VersionUnion VersionUnion::operator&&(const VersionUnion &other) const {
    auto intersect = [](const auto &pair) { return pair.first && pair.second; };
    auto notEmpty = [](const auto &x) { return !x.empty(); };

    auto result = cartesian_product(ranges, other.ranges) //
                  | std::views::transform(intersect)      //
                  | std::views::filter(notEmpty);

    std::vector<std::ranges::range_value_t<decltype(result)>> temp;
    temp.reserve(ranges.size() * other.ranges.size());
    std::ranges::copy(result, back_inserter(temp));

    return VersionUnion(std::move(temp));
}

bool VersionUnion::in(const Version &version) const {
    return ranges[0].in(version);
}

VersionUnion::VersionUnion(std::vector<VersionRange> &&ranges)
    : ranges(std::move(ranges)) {}

bool VersionUnion::empty() const { return ranges.empty(); }

VersionUnion::const_iterator VersionUnion::begin() const {
    return ranges.begin();
}

VersionUnion::const_iterator VersionUnion::end() const { return ranges.end(); }
