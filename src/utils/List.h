//
// Created by Ivan on 08.08.2023.
//

#pragma once

#include "Reactions/misc/Interface.h"

template <class T>
class List : public Interface {
  public:
    [[nodiscard]] virtual auto count() const -> int = 0;
    [[nodiscard]] virtual auto at(int index) const -> T = 0;
};
