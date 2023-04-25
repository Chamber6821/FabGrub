//
// Created by Ivan on 19.04.2023.
//

#include "semver/CustomVersion.h"
#include "doctest-nolint.h"
#include "random.h"

TEST_SUITE("CustomVersion") {
    TEST_CASE("part should be equal to source part") {
        const auto major = randomVersionPart();
        const auto minor = randomVersionPart();
        const auto patch = randomVersionPart();
        const auto version = CustomVersion{major, minor, patch};

        SUBCASE("major to source major") { CHECK_EQ(version.major(), major); }

        SUBCASE("minor to source minor") { CHECK_EQ(version.minor(), minor); }

        SUBCASE("patch to source patch") { CHECK_EQ(version.patch(), patch); }
    }
}
