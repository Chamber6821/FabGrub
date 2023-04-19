//
// Created by Ivan on 19.04.2023.
//

#include "semver/MonotonousVersion.h"
#include "doctest.h"
#include "utils.h"

TEST_SUITE("MonotonousVersion") {

    TEST_CASE("major must be equal to source part") {
        const auto part = randomVersionPart();
        const auto version = MonotonousVersion(part);

        REQUIRE_EQ(version.major(), part);
    }

    TEST_CASE("minor must be equal to source part") {
        const auto part = randomVersionPart();
        const auto version = MonotonousVersion(part);

        REQUIRE_EQ(version.minor(), part);
    }

    TEST_CASE("patch must be equal to source part") {
        const auto part = randomVersionPart();
        const auto version = MonotonousVersion(part);

        REQUIRE_EQ(version.patch(), part);
    }
}
