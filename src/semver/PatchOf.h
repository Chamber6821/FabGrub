//
// Created by Ivan on 06.08.2023.
//

#pragma once

#include "VersionPart.h"
#include "utils/parseInt.h"
#include <algorithm>
#include <fmt/format.h>
#include <string>

class PatchOf : public VersionPart {
    std::string version;

  public:
    explicit PatchOf(std::string version) : version(std::move(version)) {}

    auto value() -> int override {
        try {
            if (std::ranges::count(version, '.') < 2)
                throw std::invalid_argument("Too few dots");

            if (std::ranges::count(version, '.') > 2)
                throw std::invalid_argument("Too many dots");

            return parseIntWithoutSign(version.substr(version.rfind('.') + 1));
        } catch (...) {
            std::throw_with_nested(std::invalid_argument(fmt::format(
                "Could not get patch number of version from '{}'",
                version
            )));
        }
    }
};
