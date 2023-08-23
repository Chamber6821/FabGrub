//
// Created by Ivan on 23.08.2023.
//

#pragma once

#include "Repository.h"

#include <utility>

class OverloadedRepository : public Repository {
    ptr<Repository> origin;
    std::string _name;
    ptr<Packages> substitution;

  public:
    OverloadedRepository(
        ptr<Repository> origin, std::string name, ptr<Packages> substitution
    )
        : origin(std::move(origin)), _name(std::move(name)),
          substitution(std::move(substitution)) {}

    [[nodiscard]] auto packagesWithName(std::string name) const
        -> ptr<Packages> override {
        if (name == _name) return substitution;
        return origin->packagesWithName(name);
    }
};