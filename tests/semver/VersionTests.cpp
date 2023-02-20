//
// Created by Ivan on 05.02.2023.
//

#include "doctest.h"
#include "semver/Version.h"

TEST_SUITE_BEGIN("Version comparing");

TEST_CASE("first of all should compare by major version") {
    Version left{.major = 10, .minor = 3, .patch = 1};
    Version right{.major = 9, .minor = 6, .patch = 3};

    CHECK_FALSE((left == right));
    CHECK_FALSE((left <= right));
    CHECK_FALSE((left < right));

    CHECK((left != right));
    CHECK((left >= right));
    REQUIRE((left > right));
}

TEST_CASE("then should compare by minor version") {
    Version left{.major = 2, .minor = 3, .patch = 5};
    Version right{.major = 2, .minor = 6, .patch = 3};

    CHECK_FALSE((left == right));
    CHECK_FALSE((left >= right));
    CHECK_FALSE((left > right));

    CHECK((left != right));
    CHECK((left <= right));
    REQUIRE((left < right));
}

TEST_CASE("at the and should compare by patch version") {
    Version left{.major = 2, .minor = 6, .patch = 5};
    Version right{.major = 2, .minor = 6, .patch = 3};

    CHECK_FALSE((left == right));
    CHECK_FALSE((left <= right));
    CHECK_FALSE((left < right));

    CHECK((left != right));
    CHECK((left >= right));
    REQUIRE((left > right));
}

TEST_CASE("should be equals") {
    Version left{.major = 2, .minor = 6, .patch = 3};
    Version right{.major = 2, .minor = 6, .patch = 3};

    CHECK_FALSE((left != right));
    CHECK_FALSE((left < right));
    CHECK_FALSE((left > right));

    CHECK((left <= right));
    CHECK((left >= right));
    REQUIRE((left == right));
}

TEST_SUITE_END;
