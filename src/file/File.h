//
// Created by Ivan on 12.08.2023.
//

#pragma once

#include "Reactions/misc/Interface.h"
#include <filesystem>

class File : public Interface {
  public:
    virtual void saveTo(std::filesystem::path filepath) = 0;
};
