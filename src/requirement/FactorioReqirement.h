//
// Created by Ivan on 16.08.2023.
//

#pragma once

#include "Reactions/misc/owning/make.h"
#include "requirement/Requirement.h"
#include "semver/FactorioVersionOf.h"
#include "semver/NextPatch.h"
#include "semver/VersionOf.h"
#include <regex>

class FactorioRequirement : public Requirement {
    std::string line;
    std::regex full{R"(^~?\s*(.+?)\s*([<=>]+)\s*([0-9.]+)\s*(#.*)?$)"};
    std::regex partial{R"(^~?\s*([^#]+?)\s*(#.*)?$)"};

  public:
    explicit FactorioRequirement(std::string line) : line(std::move(line)) {}

    [[nodiscard]] auto name() const -> std::string override {
        try {
            std::smatch match;
            std::regex_match(line, match, full);
            if (match.empty()) std::regex_match(line, match, partial);
            return match[1];
        } catch (...) {
            std::throw_with_nested(std::runtime_error(
                "Failed while get name of factorio requirement"
            ));
        }
    }

    [[nodiscard]] auto low() const -> ptr<Version> override {
        try {
            std::smatch match;
            std::regex_match(line, match, full);
            if (match.empty()) return make<VersionOf>("0.0.0");

            if (match[2] == "<") return make<VersionOf>("0.0.0");
            if (match[2] == "<=") return make<VersionOf>("0.0.0");
            if (match[2] == "=") return make<FactorioVersionOf>(match[3]);
            if (match[2] == ">=") return make<FactorioVersionOf>(match[3]);
            if (match[2] == ">")
                return make<NextPatch>(make<FactorioVersionOf>(match[3]));

            throw std::runtime_error(fmt::format(
                "Invalid equality operator in requirement line \"{}\"",
                line
            ));
        } catch (...) {
            std::throw_with_nested(std::runtime_error(
                "Failed while get low version for factorio requirement"
            ));
        }
    }

    [[nodiscard]] auto high() const -> ptr<Version> override {
        try {
            const auto maxVersion = std::string("999999.999999.999999");

            std::smatch match;
            std::regex_match(line, match, full);
            if (match.empty()) return make<VersionOf>(maxVersion);

            if (match[2] == "<") return make<FactorioVersionOf>(match[3]);
            if (match[2] == "<=")
                return make<NextPatch>(make<FactorioVersionOf>(match[3]));
            if (match[2] == "=")
                return make<NextPatch>(make<FactorioVersionOf>(match[3]));
            if (match[2] == ">=") return make<VersionOf>(maxVersion);
            if (match[2] == ">") return make<VersionOf>(maxVersion);

            throw std::runtime_error(fmt::format(
                "Invalid equality operator in requirement line \"{}\"",
                line
            ));
        } catch (...) {
            std::throw_with_nested(std::runtime_error(
                "Failed while get high version for factorio requirement"
            ));
        }
    }
};
