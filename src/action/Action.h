//
// Created by Ivan on 29.08.2023.
//

#pragma once

#include "Reactions/misc/Interface.h"

class Action : public Interface {
  public:
    virtual void operator()() = 0;
};
