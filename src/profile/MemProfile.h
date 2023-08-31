//
// Created by Ivan on 08.08.2023.
//

#pragma once

#include "Profile.h"
#include <utility>

class MemProfile : public Profile {
    std::string _name;
    ptr<Version> _factorioVersion;
    ptr<Requirements> _requirements;

  public:
    MemProfile(
        std::string name, ptr<Version> factorioVersion, ptr<Requirements> reqs
    )
        : _name(std::move(name)), _factorioVersion(std::move(factorioVersion)),
          _requirements(std::move(reqs)) {}

    [[nodiscard]] auto name() const -> std::string override { return _name; }

    auto factorioVersion() const -> ptr<Version> override {
        return _factorioVersion;
    }

    [[nodiscard]] auto requirements() const -> ptr<Requirements> override {
        return _requirements;
    }
};
