//
// Created by Ivan on 08.08.2023.
//

#pragma once

#include "Reactions/misc/Interface.h"
#include "package/Package.h"

class Destination : public Interface {
  public:
    virtual void put(ptr<Package> package) = 0;
};
