//
// Created by Ivan on 12.08.2023.
//

#pragma once

#include "Reactions/misc/Interface.h"

template <class T>
class Choice : public Interface {
  public:
    [[nodiscard]] virtual auto element() const -> T = 0;
};
