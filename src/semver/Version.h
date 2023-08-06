//
// Created by Ivan on 06.08.2023.
//

#pragma once

#include "Reactions/misc/Interface.h"

class Version : public Interface {
  public:
    virtual int major() = 0;
    virtual int minor() = 0;
    virtual int patch() = 0;
};
