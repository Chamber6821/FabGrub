//
// Created by Ivan on 08.08.2023.
//

#pragma once

#include "Reactions/misc/Interface.h"
#include "package/Packages.h"
#include <string>

class Repository : public Interface {
  public:
    virtual auto packagesWithName(std::string name) -> ptr<Packages> = 0;
};
