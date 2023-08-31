//
// Created by Ivan on 23.08.2023.
//

#pragma once

#include "Repository.h"
#include <utility>

class OverloadedRepository : public Repository {
    std::string _name;
    ptr<Packages> substitution;
    ptr<Repository> origin;

  public:
    OverloadedRepository(
        std::string name, ptr<Packages> substitution, ptr<Repository> origin
    )
        : _name(std::move(name)), substitution(std::move(substitution)),
          origin(std::move(origin)) {}

    [[nodiscard]] auto packagesWithName(std::string name) const
        -> ptr<Packages> override {
        if (name == _name) return substitution;
        return origin->packagesWithName(name);
    }
};