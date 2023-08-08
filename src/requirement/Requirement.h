//
// Created by Ivan on 07.08.2023.
//

#pragma once

#include "Reactions/misc/Interface.h"
#include "semver/Version.h"
#include "utils/ptr.h"
#include <string>

class Requirement : public Interface {
  public:
    virtual auto name() -> std::string = 0;
    virtual auto low() -> ptr<Version> = 0;
    virtual auto high() -> ptr<Version> = 0;
};