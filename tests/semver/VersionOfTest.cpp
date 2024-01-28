//
// Created by Ivan on 26.04.2023.
//

#include "semver/VersionOf.h"
#include "doctest-nolint.h"
#include "random.h"
#include <fmt/format.h>
#include <numeric>
#include <ranges>
#include <string>

TEST_SUITE("VersionOf") {

    TEST_CASE("should parse random version") {
        const auto major = randomVersionPart();
        const auto minor = randomVersionPart();
        const auto patch = randomVersionPart();

        const auto version = std::to_string(major) + "." +
                             std::to_string(minor) + "." +
                             std::to_string(patch);

        auto parsed = VersionOf(version);

        SUBCASE("major should be equal to source major") {
            CHECK_EQ(parsed.major(), major);
        }

        SUBCASE("minor should be equal to source minor") {
            CHECK_EQ(parsed.minor(), minor);
        }

        SUBCASE("patch should be equal to source patch") {
            CHECK_EQ(parsed.patch(), patch);
        }
    }

    TEST_CASE("with sign before major") {
        using namespace std::literals;

        const auto major = randomVersionPart();
        const auto minor = randomVersionPart();
        const auto patch = randomVersionPart();

        const auto version = fmt::format("+{}.{}.{}", major, minor, patch);

        CAPTURE(version);

        auto parsed = VersionOf(version);

        SUBCASE("major() should throw exception") {
            CHECK_THROWS_WITH(
                (void)parsed.major(),
                fmt::format("Could not get major number of version from '{}'", version).c_str()
            );
        }

        SUBCASE("minor() should NOT throw exception") {
            CHECK_NOTHROW((void)parsed.minor());
        }

        SUBCASE("minor() should return source minor") {
            CHECK_EQ(parsed.minor(), minor);
        }

        SUBCASE("patch() should NOT throw exception") {
            CHECK_NOTHROW((void)parsed.patch());
        }

        SUBCASE("patch() should return source patch") {
            CHECK_EQ(parsed.patch(), patch);
        }
    }

    TEST_CASE("with sign before minor") {
        using namespace std::literals;

        const auto major = randomVersionPart();
        const auto minor = randomVersionPart();
        const auto patch = randomVersionPart();

        const auto version = fmt::format("{}.+{}.{}", major, minor, patch);

        CAPTURE(version);

        auto parsed = VersionOf(version);

        SUBCASE("major() should NOT throw exception") {
            CHECK_NOTHROW((void)parsed.major());
        }

        SUBCASE("major() should return source major") {
            CHECK_EQ(parsed.major(), major);
        }

        SUBCASE("minor() should throw exception") {
            CHECK_THROWS_WITH(
                (void)parsed.minor(),
                fmt::format("Could not get minor number of version from '{}'", version).c_str()
            );
        }

        SUBCASE("patch() should NOT throw exception") {
            CHECK_NOTHROW((void)parsed.patch());
        }

        SUBCASE("patch() should return source patch") {
            CHECK_EQ(parsed.patch(), patch);
        }
    }

    TEST_CASE("with sign before patch") {
        using namespace std::literals;

        const auto major = randomVersionPart();
        const auto minor = randomVersionPart();
        const auto patch = randomVersionPart();

        // NOLINTNEXTLINE
        const auto version = fmt::format("{}.{}.+{}", major, minor, patch);

        CAPTURE(version);

        auto parsed = VersionOf(version);

        SUBCASE("major() should NOT throw exception") {
            CHECK_NOTHROW((void)parsed.major());
        }

        SUBCASE("major() should return source major") {
            CHECK_EQ(parsed.major(), major);
        }

        SUBCASE("minor() should NOT throw exception") {
            CHECK_NOTHROW((void)parsed.minor());
        }

        SUBCASE("minor() should return source minor") {
            CHECK_EQ(parsed.minor(), minor);
        }

        SUBCASE("patch() should throw exception") {
            CHECK_THROWS_WITH(
                (void)parsed.patch(),
                fmt::format("Could not get patch number of version from '{}'", version).c_str()
            );
        }
    }

    TEST_CASE("few dots") {
        auto version = std::string("0.1.2.3.4.5.6");
        auto parsed = VersionOf(version);

        CAPTURE(version);

        SUBCASE("major() should not throw exception") {
            CHECK_NOTHROW((void)parsed.major());
        }

        SUBCASE("minor() should not throw exception") {
            CHECK_NOTHROW((void)parsed.minor());
        }

        SUBCASE("patch() should not throw exception") {
            CHECK_NOTHROW((void)parsed.patch());
        }
    }

    TEST_CASE("no dots") {
        auto version = std::string("0");
        auto parsed = VersionOf(version);

        CAPTURE(version);

        SUBCASE("major() should not throw exception") {
            CHECK_NOTHROW((void)parsed.major());
        }

        SUBCASE("minor() should throw exception") {
            CHECK_THROWS_WITH(
                (void)parsed.minor(),
                fmt::format("Could not get minor number of version from '{}'", version).c_str()
            );
        }

        SUBCASE("patch() should throw exception") {
            CHECK_THROWS_WITH(
                (void)parsed.patch(),
                fmt::format("Could not get patch number of version from '{}'", version).c_str()
            );
        }
    }

    TEST_CASE("single dot") {
        auto version = std::string("0.1");
        auto parsed = VersionOf(version);

        CAPTURE(version);

        SUBCASE("major() should not throw exception") {
            CHECK_NOTHROW((void)parsed.major());
        }

        SUBCASE("minor() should not throw exception") {
            CHECK_NOTHROW((void)parsed.minor());
        }

        SUBCASE("patch() should throw exception") {
            CHECK_THROWS_WITH(
                (void)parsed.patch(),
                fmt::format("Could not get patch number of version from '{}'", version).c_str()
            );
        }
    }
}
