//
// Created by Ivan on 06.08.2023.
//

#pragma once

#include "Reactions/misc/Interface.h"
#include <tuple>

class Version : public Interface {
  public:
    [[nodiscard]] virtual auto major() const -> int = 0;
    [[nodiscard]] virtual auto minor() const -> int = 0;
    [[nodiscard]] virtual auto patch() const -> int = 0;

    auto operator==(const Version &other) const {
        return std::make_tuple(major(), minor(), patch()) ==
               std::make_tuple(other.major(), other.minor(), other.patch());
    }

    auto operator<=>(const Version &other) const {
        return std::make_tuple(major(), minor(), patch()) <=>
               std::make_tuple(other.major(), other.minor(), other.patch());
    }
};
