//
// Created by Ivan on 16.08.2023.
//

#pragma once

#include "Reactions/misc/Interface.h"
#include <string>

class Http : public Interface {
  public:
    [[nodiscard]] virtual auto content(const std::string &url) const
        -> std::string = 0;
};
