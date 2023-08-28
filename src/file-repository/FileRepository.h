//
// Created by Ivan on 26.08.2023.
//

#pragma once

#include "Reactions/misc/Interface.h"
#include "package/Package.h"

class FileRepository : public Interface {
  public:
    [[nodiscard]] virtual auto fileFor(ptr<Package> package) -> ptr<File> = 0;
};
