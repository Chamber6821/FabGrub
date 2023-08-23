//
// Created by Ivan on 16.08.2023.
//

#pragma once

#include "Reactions/misc/owning/make.h"
#include "requirement/Requirement.h"
#include "semver/NextPatch.h"
#include "semver/VersionOf.h"
#include <regex>

class FactorioRequirement : public Requirement {
    std::string line;
    std::regex regex{R"(~?\s*(\S+)\s*(\S+)\s*(\S+))"};

  public:
    explicit FactorioRequirement(std::string line) : line(std::move(line)) {}

    [[nodiscard]] auto name() const -> std::string override {
        try {
            std::smatch match;
            std::regex_match(line, match, regex);
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
            std::regex_match(line, match, regex);

            if (match[2] == "<") return make<VersionOf>("0.0.0");
            if (match[2] == "<=") return make<VersionOf>("0.0.0");
            if (match[2] == "=") return make<VersionOf>(match[3]);
            if (match[2] == ">=") return make<VersionOf>(match[3]);
            if (match[2] == ">")
                return make<NextPatch>(make<VersionOf>(match[3]));

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
            std::smatch match;
            std::regex_match(line, match, regex);

            if (match[2] == "<") return make<VersionOf>(match[3]);
            if (match[2] == "<=")
                return make<NextPatch>(make<VersionOf>(match[3]));
            if (match[2] == "=")
                return make<NextPatch>(make<VersionOf>(match[3]));
            if (match[2] == ">=")
                return make<VersionOf>("999999.999999.999999");
            if (match[2] == ">") return make<VersionOf>("999999.999999.999999");

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
