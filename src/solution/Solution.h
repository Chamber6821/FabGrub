//
// Created by Ivan on 11.08.2023.
//

#pragma once

#include "Reactions/misc/Interface.h"
#include "package/Packages.h"

class Solution : public Interface {
  public:
    [[nodiscard]] virtual auto packages() const -> ptr<Packages> = 0;
};
