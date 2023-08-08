//
// Created by Ivan on 06.08.2023.
//

#pragma once

#include "Reactions/misc/Interface.h"

class Version : public Interface {
  public:
    virtual auto major() -> int = 0;
    virtual auto minor() -> int = 0;
    virtual auto patch() -> int = 0;
};
