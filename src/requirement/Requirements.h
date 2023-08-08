//
// Created by Ivan on 07.08.2023.
//

#pragma once

#include "Reactions/misc/Interface.h"
#include "Requirement.h"

class Requirements : public Interface {
  public:
    virtual auto count() -> int = 0;
    virtual auto at(int index) -> ptr<Requirement> = 0;
};
