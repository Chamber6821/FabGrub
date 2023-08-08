//
// Created by Ivan on 06.08.2023.
//

#pragma once

#include "Reactions/misc/Interface.h"
#include "requirement/Requirements.h"
#include "semver/Version.h"
#include <string>
#include <vector>

class Package : public Interface {
  public:
    virtual auto name() -> std::string = 0;
    virtual auto version() -> ptr<Version> = 0;
    virtual auto requirements() -> ptr<Requirements> = 0;
    virtual auto data() -> std::string = 0;
};
