//
// Created by Ivan on 12.08.2023.
//

#pragma once

#include "ChosenPackage.h"
#include "Packages.h"
#include "Reactions/misc/owning/make.h"
#include "choice/CachedChoice.h"
#include "choice/Choice.h"
#include "choice/SuitablePackageWithVersion.h"
#include "fmt/format.h"
#include "fmt/ostream.h"
#include <utility>

class PackageWithVersion : public ChosenPackage {
  public:
    PackageWithVersion(ptr<Packages> packages, ptr<Version> version)
        : ChosenPackage(make<CachedChoice<ptr<Package>>>(
              std::make_shared<SuitablePackageWithVersion>(
                  std::move(packages), std::move(version)
              )
          )) {}
};
