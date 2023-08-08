//
// Created by Ivan on 06.08.2023.
//

#pragma once

#include "Reactions/misc/Interface.h"
#include <tuple>

class Version : public Interface {
  public:
    virtual auto major() -> int = 0;
    virtual auto minor() -> int = 0;
    virtual auto patch() -> int = 0;

    auto operator==(Version &other) {
        return std::make_tuple(major(), minor(), patch()) ==
               std::make_tuple(other.major(), other.minor(), other.patch());
    }

    auto operator<=>(Version &other) {
        return std::make_tuple(major(), minor(), patch()) <=>
               std::make_tuple(other.major(), other.minor(), other.patch());
    }
};
