//
// Created by Ivan on 06.08.2023.
//

#pragma once

#include "VersionPart.h"
#include "utils/parseInt.h"
#include <algorithm>
#include <fmt/format.h>
#include <regex>
#include <string>

class PatchOf : public VersionPart {
    std::regex regex{R"(^[^.]*\.[^.]*\.(\d+))"};
    std::string version;

  public:
    explicit PatchOf(std::string version) : version(std::move(version)) {}

    auto value() -> int override {
        try {
            std::smatch match;
            if (not std::regex_search(version, match, regex))
                throw std::runtime_error(
                    fmt::format("Could not get number from '{}'", version)
                );

            return parseIntWithoutSign({match[1].first, match[1].second});
        } catch (...) {
            std::throw_with_nested(std::invalid_argument(fmt::format(
                "Could not get patch number of version from '{}'",
                version
            )));
        }
    }
};
