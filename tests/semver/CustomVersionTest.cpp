//
// Created by Ivan on 19.04.2023.
//

#include "semver/CustomVersion.h"
#include "doctest.h"
#include "random.h"

TEST_SUITE("CustomVersion") {
    const auto major = randomVersionPart();
    const auto minor = randomVersionPart();
    const auto patch = randomVersionPart();
    const auto version = CustomVersion(major, minor, patch);

    TEST_CASE("major should be equal to source major part") {
        REQUIRE_EQ(version.major(), major);
    }

    TEST_CASE("minor should be equal to source minor part") {
        REQUIRE_EQ(version.minor(), minor);
    }

    TEST_CASE("patch should be equal to source patch part") {
        REQUIRE_EQ(version.patch(), patch);
    }
}
