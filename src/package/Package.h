//
// Created by Ivan on 06.08.2023.
//

#pragma once

#include "Reactions/misc/Interface.h"
#include "file/File.h"
#include "requirement/Requirements.h"
#include "semver/Version.h"
#include <string>
#include <vector>

class Package : public Interface {
  public:
    [[nodiscard]] virtual auto name() const -> std::string = 0;
    [[nodiscard]] virtual auto version() const -> ptr<Version> = 0;
    [[nodiscard]] virtual auto requirements() const -> ptr<Requirements> = 0;
    [[nodiscard]] virtual auto file() const -> ptr<File> = 0;
};
