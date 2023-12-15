//
// Created by Ivan on 08.08.2023.
//

#pragma once

#include "Profile.h"
#include <utility>

class MemProfile : public Profile {
    ptr<Requirements> _requirements;

  public:
    explicit MemProfile(ptr<Requirements> reqs)
        : _requirements(std::move(reqs)) {}

    [[nodiscard]] auto requirements() const -> ptr<Requirements> override {
        return _requirements;
    }
};
