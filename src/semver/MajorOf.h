//
// Created by Ivan on 06.08.2023.
//

#pragma once

#include "VersionPart.h"
#include "utils/parseInt.h"
#include <algorithm>
#include <fmt/format.h>
#include <string>
#include <utility>

class MajorOf : public VersionPart {
    std::string version;

  public:
    explicit MajorOf(std::string version) : version(std::move(version)) {}

    auto value() -> int override {
        try {
            if (std::ranges::count(version, '.') < 2)
                throw std::invalid_argument("Too few dots");

            if (std::ranges::count(version, '.') > 2)
                throw std::invalid_argument("Too many dots");

            return parseIntWithoutSign(version.substr(0, version.find('.')));
        } catch (...) {
            std::throw_with_nested(std::invalid_argument(fmt::format(
                "Could not get major number of version from '{}'",
                version
            )));
        }
    }
};
