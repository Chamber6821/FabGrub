//
// Created by Ivan on 06.08.2023.
//

#pragma once

#include "VersionPart.h"
#include "utils/parseInt.h"
#include <algorithm>
#include <fmt/format.h>
#include <string>

class MinorOf : public VersionPart {
    std::string version;

  public:
    explicit MinorOf(std::string version) : version(std::move(version)) {}

    auto value() -> int override {
        try {
            if (std::ranges::count(version, '.') < 2)
                throw std::invalid_argument("Too few dots");

            if (std::ranges::count(version, '.') > 2)
                throw std::invalid_argument("Too many dots");

            const auto firstDot = version.find('.');
            const auto lastDot = version.rfind('.');
            const auto start = firstDot + 1;
            return parseIntWithoutSign(version.substr(start, lastDot - start));
        } catch (...) {
            std::throw_with_nested(std::invalid_argument(fmt::format(
                "Could not get minor number of version from '{}'",
                version
            )));
        }
    }
};
