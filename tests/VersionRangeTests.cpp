//
// Created by Ivan on 05.02.2023.
//

#include "VersionRange.h"
#include <doctest.h>
#include <limits>
#include <ostream>
#include <random>

Version getRandomVersion() {
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_int_distribution des{0, std::numeric_limits<int>::max()};

    return {.major = des(gen), .minor = des(gen), .patch = des(gen)};
}

Version getRandomVersionLessThan(Version barrier) {
    auto result = getRandomVersion();
    while (result >= barrier)
        result = getRandomVersion();
    return result;
}

Version getRandomVersionGreaterThan(Version barrier) {
    auto result = getRandomVersion();
    while (result <= barrier)
        result = getRandomVersion();
    return result;
}

Version getRandomVersionBetween(Version left, Version right) {
    auto result = getRandomVersion();
    while (!(left < result && result < right))
        result = getRandomVersion();
    return result;
}

std::ostream &operator<<(std::ostream &out, const Version &version) {
    return out << version.major << '.' << version.minor << '.' << version.patch;
}

std::ostream &operator<<(std::ostream &out, const VersionRange &range) { return out << range.from << '-' << range.to; }

TEST_SUITE("VersionRange") {
    TEST_CASE("method in") {
        Version lowest = getRandomVersion();
        Version highest = getRandomVersionGreaterThan(lowest);

        VersionRange testRange{lowest, highest};
        CAPTURE(testRange);

        SUBCASE("should return false for too small version") {
            auto small = getRandomVersionLessThan(lowest);
            CAPTURE(small);
            CHECK_FALSE(testRange.in(small));
        }

        SUBCASE("should return true for version on left bound") {
            CAPTURE(lowest);
            CHECK(testRange.in(lowest));
        }

        SUBCASE("should return true for version between lowest and highest") {
            auto middle = getRandomVersionBetween(lowest, highest);
            CAPTURE(middle);
            CHECK(testRange.in(middle));
        }

        SUBCASE("should return true for version on right bound") {
            CAPTURE(highest);
            CHECK(testRange.in(highest));
        }

        SUBCASE("should return false for too small version") {
            auto big = getRandomVersionGreaterThan(highest);
            CAPTURE(big);
            CHECK_FALSE(testRange.in(big));
        }
    }

    TEST_CASE("operator 'and' should return intersection of ranges") {
        auto low1 = getRandomVersion();
        auto high1 = getRandomVersionGreaterThan(low1);
        VersionRange range1{low1, high1};
        CAPTURE(range1);

        auto low2 = getRandomVersion();
        auto high2 = getRandomVersionGreaterThan(low2);
        VersionRange range2{low2, high2};
        CAPTURE(range2);

        VersionRange expectedIntersection{std::max(low1, low2), std::min(high1, high2)};

        REQUIRE_EQ(expectedIntersection, range1 && range2);
    }

    TEST_CASE("method empty") {
        auto left = getRandomVersion();
        auto right = getRandomVersionGreaterThan(left);

        SUBCASE("should be not empty if right bound greater left bound") {
            VersionRange range{left, right};
            CAPTURE(range);
            CHECK_FALSE(range.empty());
        }

        SUBCASE("should be not empty if right bound equal to left bound") {
            VersionRange range{left, left};
            CAPTURE(range);
            CHECK_FALSE(range.empty());
        }

        SUBCASE("should be empty if right bound less than left bound") {
            VersionRange range{right, left};
            CAPTURE(range);
            CHECK(range.empty());
        }
    }
}

TEST_SUITE("VersionRange fabric methods") {
    TEST_CASE("greaterThan should return range with right border equal to positive_infinity") {
        auto version = getRandomVersion();
        auto test = VersionRange::greaterThan(version);

        auto left = version;
        left.patch++;

        REQUIRE_EQ(test, VersionRange{left, VersionRange::positive_infinity});
    }

    TEST_CASE("greaterOrEqualsThan should return range with right border equal to positive_infinity") {
        auto version = getRandomVersion();
        auto test = VersionRange::greaterOrEqualsThan(version);

        REQUIRE_EQ(test, VersionRange{version, VersionRange::positive_infinity});
    }

    TEST_CASE("equals should return range contains one version") {
        auto version = getRandomVersion();
        auto test = VersionRange::equals(version);

        REQUIRE_EQ(test, VersionRange{version, version});
    }

    TEST_CASE("lessOrEqualsThan should return range with left border equal to negative_infinity") {
        auto version = getRandomVersion();
        auto test = VersionRange::lessOrEqualsThan(version);

        REQUIRE_EQ(test, VersionRange{VersionRange::negative_infinity, version});
    }

    TEST_CASE("lessThan should return range with left border equal to negative_infinity") {
        auto version = getRandomVersion();
        auto test = VersionRange::lessThan(version);

        auto left = version;
        left.patch--;

        REQUIRE_EQ(test, VersionRange{VersionRange::negative_infinity, left});
    }
}
