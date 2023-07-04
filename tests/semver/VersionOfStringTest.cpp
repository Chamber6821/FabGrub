//
// Created by Ivan on 26.04.2023.
//

#include "semver/VersionOfString.h"
#include "doctest-nolint.h"
#include "random.h"
#include <fmt/format.h>
#include <numeric>
#include <ranges>

TEST_SUITE("VersionOfString") {

    auto exceptionMessage(std::string_view invalidVersion)->std::string {
        return fmt::format("Could not parse '{}' version", invalidVersion);
    }

    TEST_CASE("should parse random version") {
        const auto major = randomVersionPart();
        const auto minor = randomVersionPart();
        const auto patch = randomVersionPart();

        const auto version = std::to_string(major) + "." +
                             std::to_string(minor) + "." +
                             std::to_string(patch);

        const auto parsed = VersionOfString(version);

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

        const auto parsed = VersionOfString(version);

        SUBCASE("major() should throw exception") {
            CHECK_THROWS_WITH((void)parsed.major(),
                              exceptionMessage(version).c_str());
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

        const auto parsed = VersionOfString(version);

        SUBCASE("major() should NOT throw exception") {
            CHECK_NOTHROW((void)parsed.major());
        }

        SUBCASE("major() should return source major") {
            CHECK_EQ(parsed.major(), major);
        }

        SUBCASE("minor() should throw exception") {
            CHECK_THROWS_WITH((void)parsed.minor(),
                              exceptionMessage(version).c_str());
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

        const auto version = fmt::format("{}.{}.+{}", major, minor, patch);

        CAPTURE(version);

        const auto parsed = VersionOfString(version);

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
            CHECK_THROWS_WITH((void)parsed.patch(),
                              exceptionMessage(version).c_str());
        }
    }

    TEST_CASE("dots more than 2") {
        using namespace std::views; // NOLINT(google-build-using-namespace)

        const auto minSegmentsCount = Min{4};
        const auto maxSegmentsCount = Max{10};

        auto generated =                                          //
            iota(0, random(minSegmentsCount, maxSegmentsCount))   //
            | transform([](auto x) { return std::to_string(x); }) //
            | transform([](auto x) { return "." + x; })           //
            | join                                                //
            | drop(1); // drop the first extra dot

        std::string version;
        std::ranges::copy(generated, std::back_inserter(version));

        CAPTURE(version);

        const auto parsed = VersionOfString(version);

        SUBCASE("major() should throw exception") {
            CHECK_THROWS_WITH((void)parsed.major(),
                              exceptionMessage(version).c_str());
        }

        SUBCASE("minor() should throw exception") {
            CHECK_THROWS_WITH((void)parsed.minor(),
                              exceptionMessage(version).c_str());
        }

        SUBCASE("patch() should throw exception") {
            CHECK_THROWS_WITH((void)parsed.patch(),
                              exceptionMessage(version).c_str());
        }
    }

    TEST_CASE("dots less than 2") {
        using namespace std::views; // NOLINT(google-build-using-namespace)

        const auto minSegmentsCount = Min{0};
        const auto maxSegmentsCount = Max{2};

        auto generated =                                          //
            iota(0, random(minSegmentsCount, maxSegmentsCount))   //
            | transform([](auto x) { return std::to_string(x); }) //
            | transform([](auto x) { return "." + x; })           //
            | join                                                //
            | drop(1); // drop the first extra dot

        std::string version;
        std::ranges::copy(generated, std::back_inserter(version));

        CAPTURE(version);

        const auto parsed = VersionOfString(version);

        SUBCASE("major() should throw exception") {
            CHECK_THROWS_WITH((void)parsed.major(),
                              exceptionMessage(version).c_str());
        }

        SUBCASE("minor() should throw exception") {
            CHECK_THROWS_WITH((void)parsed.minor(),
                              exceptionMessage(version).c_str());
        }

        SUBCASE("patch() should throw exception") {
            CHECK_THROWS_WITH((void)parsed.patch(),
                              exceptionMessage(version).c_str());
        }
    }
}
