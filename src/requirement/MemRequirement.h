//
// Created by Ivan on 07.08.2023.
//

#pragma once

#include "Reactions/misc/owning/make.h"
#include "Requirement.h"

#include "semver/VersionOf.h"
#include <utility>

class MemRequirement : public Requirement {
    std::string _name;
    ptr<Version> _low;
    ptr<Version> _high;

  public:
    MemRequirement(std::string name, ptr<Version> low, ptr<Version> high)
        : _name(std::move(name)), _low(std::move(low)), _high(std::move(high)) {
    }

    MemRequirement(
        std::string name, const std::string &low, const std::string &high
    )
        : MemRequirement(
              std::move(name), make<VersionOf>(low), make<VersionOf>(high)
          ) {}

    [[nodiscard]] auto name() const -> std::string override { return _name; }

    [[nodiscard]] auto low() const -> ptr<Version> override { return _low; }

    [[nodiscard]] auto high() const -> ptr<Version> override { return _high; };
};
