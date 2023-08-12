//
// Created by Ivan on 08.08.2023.
//

#pragma once

#include "Reactions/misc/Interface.h"
#include "package/Packages.h"

class Destination : public Interface {
  public:
    virtual void fill(ptr<Packages> filling) = 0;
};
