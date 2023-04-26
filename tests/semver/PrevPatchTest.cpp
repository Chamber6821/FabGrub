//
// Created by Ivan on 26.04.2023.
//

#include "semver/PrevPatch.h"
#include "doctest-nolint.h"
#include "random.h"
#include "semantic-type/Max.h"
#include "semantic-type/Min.h"
#include "semver/CustomVersion.h"

TEST_SUITE("PrevPatch") {
    TEST_CASE("no overflowing") {
        const auto min = Min{VersionPart{1}};
        const auto max = Max{VersionPart{100}};

        const auto major = randomVersionPart(min, max);
        const auto minor = randomVersionPart(min, max);
        const auto patch = randomVersionPart(min, max);

        const auto prev = PrevPatch(CustomVersion(major, minor, patch));

        SUBCASE("major should be constant") { CHECK_EQ(prev.major(), major); }

        SUBCASE("minor should be constant") { CHECK_EQ(prev.minor(), minor); }

        SUBCASE("patch should be less by one") {
            CHECK_EQ(prev.patch(), patch - 1);
        }
    }

    TEST_CASE("patch is min") {
        const auto major = randomVersionPart();
        const auto minor = randomVersionPart(Min{1}, MaxVersionPart);
        const auto patch = MinVersionPart.value();

        const auto prev = PrevPatch(CustomVersion(major, minor, patch));

        SUBCASE("major should be constant") { CHECK_EQ(prev.major(), major); }

        SUBCASE("minor should be less by one") {
            CHECK_EQ(prev.minor(), minor - 1);
        }

        SUBCASE("patch should be max") {
            CHECK_EQ(prev.patch(), MaxVersionPart.value());
        }
    }

    TEST_CASE("patch and minor is min") {
        const auto min = Min{VersionPart{0}};
        const auto max = Max{VersionPart{100}};

        const auto major = randomVersionPart(Min{1}, max);
        const auto minor = min.value();
        const auto patch = min.value();

        const auto prev =
            PrevPatch(CustomVersion(major, minor, patch), min, max);

        SUBCASE("major should be less by one") {
            CHECK_EQ(prev.major(), major - 1);
        }

        SUBCASE("minor should be max") { CHECK_EQ(prev.minor(), max.value()); }

        SUBCASE("patch should be max") { CHECK_EQ(prev.patch(), max.value()); }
    }

    TEST_CASE("patch and minor and major is min") {
        const auto min = Min{VersionPart{0}};
        const auto max = Max{VersionPart{100}};

        const auto major = min.value();
        const auto minor = min.value();
        const auto patch = min.value();

        const auto next =
            PrevPatch(CustomVersion(major, minor, patch), min, max);

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
