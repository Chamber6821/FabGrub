#pragma once

#include "VersionPart.h"
#include "fmt/format.h"
#include "utils/parseInt.h"
#include <algorithm>
#include <stdexcept>
#include <string>
#include <utility>

class OptionalPatch : public VersionPart {
    std::string version;
    int otherwise;

  public:
    OptionalPatch(std::string version, int otherwise)
        : version(std::move(version)), otherwise(otherwise) {}

    auto value() -> int override {
        try {
            auto dotsCount = std::ranges::count(version, '.');
            if (dotsCount == 1) return otherwise;
            if (dotsCount > 2) throw std::invalid_argument("Too many dots");
            return parseIntWithoutSign(version.substr(version.rfind('.') + 1));
        } catch (...) {
            std::throw_with_nested(std::invalid_argument(fmt::format(
                "Could not get patch number of version from '{}'",
                version
            )));
        }
    }
};
