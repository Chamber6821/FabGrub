//
// Created by Ivan on 25.04.2023.
//

#include "semver/NextPatch.h"
#include "doctest-nolint.h"
#include "random.h"
#include "semver/CustomVersion.h"

TEST_SUITE("NextPatch") {
    TEST_CASE("no overflowing") {
        const auto min = Min{VersionPart{0}};
        const auto max = Max{VersionPart{100}};
        const auto overflowBorder = Max{VersionPart{101}};

        const auto major = randomVersionPart(min, max);
        const auto minor = randomVersionPart(min, max);
        const auto patch = randomVersionPart(min, max);

        CAPTURE(major);
        CAPTURE(minor);
        CAPTURE(patch);
        CAPTURE(min.value());
        CAPTURE(max.value());
        CAPTURE(overflowBorder.value());

        const auto next =
            NextPatch(CustomVersion(major, minor, patch), min, overflowBorder);

        SUBCASE("major should be constant") { CHECK_EQ(next.major(), major); }

        SUBCASE("minor should be constant") { CHECK_EQ(next.minor(), minor); }

        SUBCASE("patch should be larger by one") {
            CHECK_EQ(next.patch(), patch + 1);
        }
    }

    TEST_CASE("patch is max") {
        const auto min = Min{VersionPart{0}};
        const auto safeMax = Max{VersionPart{100}};
        const auto overflowBorder = Max{VersionPart{101}};

        const auto major = randomVersionPart(min, safeMax);
        const auto minor = randomVersionPart(min, safeMax);
        const auto patch = overflowBorder.value();

        CAPTURE(major);
        CAPTURE(minor);
        CAPTURE(patch);
        CAPTURE(min.value());
        CAPTURE(safeMax.value());
        CAPTURE(overflowBorder.value());

        const auto next =
            NextPatch(CustomVersion(major, minor, patch), min, overflowBorder);

        SUBCASE("major should be constant") { CHECK_EQ(next.major(), major); }

        SUBCASE("minor should be larger by one") {
            CHECK_EQ(next.minor(), minor + 1);
        }

        SUBCASE("patch should be min") { CHECK_EQ(next.patch(), min.value()); }
    }

    TEST_CASE("patch and minor is max") {
        const auto min = Min{VersionPart{0}};
        const auto safeMax = Max{VersionPart{100}};
        const auto overflowBorder = Max{VersionPart{101}};

        const auto major = randomVersionPart(min, safeMax);
        const auto minor = overflowBorder.value();
        const auto patch = overflowBorder.value();

        CAPTURE(major);
        CAPTURE(minor);
        CAPTURE(patch);
        CAPTURE(min.value());
        CAPTURE(safeMax.value());
        CAPTURE(overflowBorder.value());

        const auto next =
            NextPatch(CustomVersion(major, minor, patch), min, overflowBorder);

        SUBCASE("major should be larger by one") {
            CHECK_EQ(next.major(), major + 1);
        }

        SUBCASE("minor should be min") { CHECK_EQ(next.minor(), min.value()); }

        SUBCASE("patch should be min") { CHECK_EQ(next.patch(), min.value()); }
    }

    TEST_CASE("patch and minor and major is max") {
        const auto min = Min{VersionPart{0}};
        const auto overflowBorder = Max{VersionPart{100}};

        const auto major = overflowBorder.value();
        const auto minor = overflowBorder.value();
        const auto patch = overflowBorder.value();

        CAPTURE(major);
        CAPTURE(minor);
        CAPTURE(patch);
        CAPTURE(min.value());
        CAPTURE(overflowBorder.value());

        const auto next =
            NextPatch(CustomVersion(major, minor, patch), min, overflowBorder);

        SUBCASE("major() should throw exception") {
            CHECK_THROWS((void)next.major());
        }

        SUBCASE("minor() should throw exception") {
            CHECK_THROWS((void)next.minor());
        }

        SUBCASE("patch() should throw exception") {
            CHECK_THROWS((void)next.patch());
        }
    }
}
