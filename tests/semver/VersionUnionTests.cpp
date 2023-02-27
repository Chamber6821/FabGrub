//
// Created by Ivan on 21.02.2023.
//

#include "semver/VersionUnion.h"
#include "utils.h"
#include <concepts>
#include <doctest.h>

static std::ostream &operator<<(std::ostream &out, const Version &version) {
    return out << version.major << '.' << version.minor << '.' << version.patch;
}

static std::ostream &operator<<(std::ostream &out, const VersionRange &range) {
    return out << range.from() << '-' << range.to();
}

std::ostream &operator<<(std::ostream &out, const VersionUnion &aUnion) {
    if (aUnion.ranges.empty())
        return out << "<empty>";

    auto it = aUnion.ranges.begin();
    out << "(" << *(it++) << ")";
    for (; it != aUnion.ranges.end(); ++it) {
        out << " U (" << aUnion.ranges.back() << ")";
    }
    return out;
}

template <typename FuncType, typename RetType>
concept Generator = requires(FuncType func) {
                        { func() } -> std::same_as<RetType>;
                    };

template <typename T, typename F>
    requires Generator<F, T>
T getRandomNotEquals(F generator, const T &standard) {
    T result = generator();
    while (result == standard)
        result = generator();
    return result;
}

TEST_SUITE_BEGIN("VersionUnion");

TEST_CASE("should be equal to self") {
    VersionUnion test(getRandomRange());

    REQUIRE_EQ(test, test);
}

TEST_CASE("should not be equals to another union") {
    auto firstRange = getRandomRange();
    auto secondRange = getRandomNotEquals(getRandomRange, firstRange);

    VersionUnion first(firstRange);
    VersionUnion second(secondRange);

    REQUIRE_NE(first, second);
}

TEST_CASE("should return true if version in single range") {
    auto left = getRandomVersion();
    auto right = getRandomVersionGreaterThan(left);
    auto testVersion = getRandomVersionBetween(left, right);

    VersionUnion testUnion({left, right});

    CAPTURE(testUnion);
    CAPTURE(testVersion);

    REQUIRE(testUnion.in(testVersion));
}

TEST_CASE("should return false if version out single range") {
    auto left = getRandomVersion();
    auto right = getRandomVersionGreaterThan(left);

    VersionUnion testUnion({left, right});

    CAPTURE(testUnion);

    SUBCASE("more to the right than range") {
        auto testVersion = getRandomVersionGreaterThan(right);
        CAPTURE(testVersion);

        CHECK_FALSE(testUnion.in(testVersion));
    }

    SUBCASE("more to the left than range") {
        auto testVersion = getRandomVersionLessThan(left);
        CAPTURE(testVersion);

        CHECK_FALSE(testUnion.in(testVersion));
    }
}

TEST_CASE("should return false if version out two not intersected ranges") {
    auto leftLeft = getRandomVersion();
    auto leftRight = getRandomVersionGreaterThan(leftLeft);
    auto rightLeft = getRandomVersionGreaterThan(leftRight);
    auto rightRight = getRandomVersionGreaterThan(rightLeft);

    VersionUnion left({leftLeft, leftRight});
    VersionUnion right({rightLeft, rightRight});
    CAPTURE(left);
    CAPTURE(right);

    VersionUnion testUnion = left || right;
    CAPTURE(testUnion);

    SUBCASE("version between ranges") {
        auto testVersion = getRandomVersionBetween(leftRight, rightLeft);
        CAPTURE(testVersion);
        CHECK_FALSE(testUnion.in(testVersion));
    }

    SUBCASE("version more the left") {
        auto testVersion = getRandomVersionLessThan(leftLeft);
        CAPTURE(testVersion);
        CHECK_FALSE(testUnion.in(testVersion));
    }

    SUBCASE("version more the right") {
        auto testVersion = getRandomVersionGreaterThan(rightRight);
        CAPTURE(testVersion);
        CHECK_FALSE(testUnion.in(testVersion));
    }
}

TEST_CASE("should unite intersect ranges") {
    auto v1 = getRandomVersion();
    auto v2 = getRandomVersionGreaterThan(v1);
    auto v3 = getRandomVersionGreaterThan(v2);
    auto v4 = getRandomVersionGreaterThan(v3);

    VersionUnion left({v1, v3});
    VersionUnion right({v2, v4});

    VersionUnion expected({v1, v4});

    CAPTURE(left);
    CAPTURE(right);
    REQUIRE_EQ((left || right), expected);
}

TEST_CASE("should unite not intersect ranges to one range") {
    auto v1 = getRandomVersion();
    auto v2 = getRandomVersionGreaterThan(v1);
    auto v3 = getRandomVersionGreaterThan(v2);
    auto v4 = getRandomVersionGreaterThan(v3);

    auto spaceLeft = getRandomVersionBetween(v1, v2);
    auto spaceRight = getRandomVersionBetween(v3, v4);

    VersionUnion left({v1, v2});
    VersionUnion right({v3, v4});

    VersionUnion anUnion = left || right;
    VersionUnion space({spaceLeft, spaceRight});

    VersionUnion expected({v1, v4});

    CAPTURE(left);
    CAPTURE(right);
    CAPTURE(anUnion);
    CAPTURE(space);
    REQUIRE_EQ((anUnion || space), expected);
}

TEST_CASE("should filter empty ranges") {
    VersionUnion empty(VersionRange::nothing);
    VersionUnion origin(getRandomNotEmptyRange());

    REQUIRE_EQ((origin || empty), origin);
}

TEST_CASE("should return empty union of empty ranges") {
    REQUIRE_EQ((VersionUnion::nothing() || VersionUnion::nothing()),
               VersionUnion::nothing());
}

TEST_CASE("should return intersection of single ranges") {
    auto v1 = getRandomVersion();
    auto v2 = getRandomVersionGreaterThan(v1);
    auto v3 = getRandomVersionGreaterThan(v2);
    auto v4 = getRandomVersionGreaterThan(v3);

    VersionUnion left({v1, v3});
    VersionUnion right({v2, v4});

    VersionUnion expected({v2, v3});

    CAPTURE(left);
    CAPTURE(right);
    REQUIRE_EQ((left && right), expected);
}

TEST_CASE("operator 'and' should be symmetric") {
    auto v1 = getRandomVersion();
    auto v2 = getRandomVersionGreaterThan(v1);
    auto v3 = getRandomVersionGreaterThan(v2);
    auto v4 = getRandomVersionGreaterThan(v3);

    VersionUnion left({v1, v3});
    VersionUnion right({v2, v4});

    CAPTURE(left);
    CAPTURE(right);
    REQUIRE_EQ((left && right), (right && left));
}

TEST_CASE("operator 'and' for intersected edges and center") {
    auto v1 = getRandomVersion();
    auto v2 = getRandomVersionGreaterThan(v1);
    auto v3 = getRandomVersionGreaterThan(v2);
    auto v4 = getRandomVersionGreaterThan(v3);
    auto v5 = getRandomVersionGreaterThan(v4);
    auto v6 = getRandomVersionGreaterThan(v5);

    VersionUnion leftPart({v1, v3});
    VersionUnion rightPart({v4, v6});
    VersionUnion edges = leftPart || rightPart;
    CAPTURE(edges);

    VersionUnion center({v2, v5});
    CAPTURE(center);

    SUBCASE("should return expected intersection") {
        VersionUnion leftIntersection({v2, v3});
        VersionUnion rightIntersection({v4, v5});
        VersionUnion expected = leftIntersection || rightIntersection;

        CHECK_EQ((edges && center), expected);
    }

    SUBCASE("should be symmetric") {
        CHECK_EQ((edges && center), (center && edges));
    }
}

TEST_CASE("should return empty intersection") {
    auto v1 = getRandomVersion();
    auto v2 = getRandomVersionGreaterThan(v1);
    auto v3 = getRandomVersionGreaterThan(v2);
    auto v4 = getRandomVersionGreaterThan(v3);

    VersionUnion left({v1, v2});
    VersionUnion right({v3, v4});

    CAPTURE(left);
    CAPTURE(right);

    REQUIRE_EQ((left && right), VersionUnion(VersionRange::nothing));
}

TEST_CASE("should return true if equals VersionUnion::nothing()") {
    REQUIRE(VersionUnion::nothing().empty());
}

TEST_CASE("should return false if not equals VersionUnion::nothing()") {
    auto myUnion = VersionUnion(getRandomNotEmptyRange());
    CAPTURE(myUnion);
    REQUIRE_FALSE(myUnion.empty());
}

TEST_SUITE_END;
