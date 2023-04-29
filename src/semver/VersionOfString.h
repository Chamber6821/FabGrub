//
// Created by Ivan on 18.04.2023.
//

#pragma once

#include "contract/semver/Version.h"
#include "utils/parseInt.h"
#include "utils/pure.h"
#include <algorithm>
#include <format>
#include <string_view>

// Parse the string with the version looks like '1.0.0' (without spaces)
class VersionOfString final {
    std::string_view string;

  public:
    constexpr explicit VersionOfString(std::string_view string)
        : string(string) {}

    [[nodiscard]] pure major() const try {
        if (std::ranges::count(string, '.') != 2)
            throw std::invalid_argument("Too many dots");

        return parseIntWithoutSign(string.substr(0, string.find('.')));
    } catch (...) {

        std::throw_with_nested(std::invalid_argument(
            std::format("Could not parse '{}' version", string)));
    }

    [[nodiscard]] pure minor() const try {
        if (std::ranges::count(string, '.') != 2)
            throw std::invalid_argument("Too many dots");

        const auto firstDot = string.find('.');
        const auto lastDot = string.rfind('.');
        const auto start = firstDot + 1;
        return parseIntWithoutSign(string.substr(start, lastDot - start));
    } catch (...) {

        std::throw_with_nested(std::invalid_argument(
            std::format("Could not parse '{}' version", string)));
    }

    [[nodiscard]] pure patch() const try {
        if (std::ranges::count(string, '.') != 2)
            throw std::invalid_argument("Too many dots");

        return parseIntWithoutSign(string.substr(string.rfind('.') + 1));
    } catch (...) {

        std::throw_with_nested(std::invalid_argument(
            std::format("Could not parse '{}' version", string)));
    }
};

static_assert(Version<VersionOfString>);
