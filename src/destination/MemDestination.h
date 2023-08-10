//
// Created by Ivan on 08.08.2023.
//

#pragma once

#include "Destination.h"

#include "package/Packages.h"
#include <utility>

class MemDestination : public Destination, public Packages {
    std::vector<ptr<Package>> packages;

  public:
    explicit MemDestination(std::vector<ptr<Package>> packages)
        : packages(std::move(packages)) {}

    MemDestination() : MemDestination(std::vector<ptr<Package>>{}) {}

    void put(ptr<Package> package) override { packages.push_back(package); }

    auto count() -> int override { return static_cast<int>(packages.size()); }

    auto at(int index) -> ptr<Package> override { return packages.at(index); }
};