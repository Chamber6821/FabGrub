//
// Created by Ivan on 08.08.2023.
//

#pragma once

#include "Profile.h"
#include <utility>

class MemProfile : public Profile {
    std::string _name;
    ptr<Requirements> reqs;

  public:
    MemProfile(std::string name, ptr<Requirements> reqs)
        : _name(std::move(name)), reqs(std::move(reqs)) {}

    [[nodiscard]] auto name() const -> std::string override { return _name; }

    [[nodiscard]] auto requirements() const -> ptr<Requirements> override {
        return reqs;
    }
};
