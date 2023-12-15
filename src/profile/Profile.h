//
// Created by Ivan on 08.08.2023.
//

#pragma once

#include "Reactions/misc/Interface.h"
#include "requirement/Requirements.h"
#include "utils/ptr.h"
#include <string>

class Profile : public Interface {
  public:
    [[nodiscard]] virtual auto requirements() const -> ptr<Requirements> = 0;
};
