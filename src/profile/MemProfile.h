//
// Created by Ivan on 08.08.2023.
//

#pragma once

#include "Profile.h"
#include <utility>

class MemProfile : public Profile {
    ptr<Version> _factorioVersion;
    ptr<Requirements> _requirements;

  public:
    MemProfile(ptr<Version> factorioVersion, ptr<Requirements> reqs)
        : _factorioVersion(std::move(factorioVersion)),
          _requirements(std::move(reqs)) {}

    [[nodiscard]] auto factorioVersion() const -> ptr<Version> override {
        return _factorioVersion;
    }

    [[nodiscard]] auto requirements() const -> ptr<Requirements> override {
        return _requirements;
    }
};
