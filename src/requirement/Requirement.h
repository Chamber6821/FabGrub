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
    [[nodiscard]] virtual auto name() const -> std::string = 0;
    [[nodiscard]] virtual auto low() const -> ptr<Version> = 0;
    [[nodiscard]] virtual auto high() const -> ptr<Version> = 0;
};