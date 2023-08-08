//
// Created by Ivan on 08.08.2023.
//

#pragma once

#include "Reactions/misc/Interface.h"

template<class T>
class List : public Interface {
  public:
    virtual auto count() -> int = 0;
    virtual auto at(int index) -> T = 0;
};
