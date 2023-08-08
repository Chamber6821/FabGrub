//
// Created by Ivan on 08.08.2023.
//

#pragma once

#include "Reactions/misc/owning/make.h"
#include "Repository.h"
#include "package/MemPackages.h"
#include <map>
#include <utility>

class MemRepository : public Repository {
  public:
    using Map = std::map<std::string, ptr<Packages>>;

  private:
    Map packageMap;

  public:
    explicit MemRepository(Map packageMap)
        : packageMap(std::move(packageMap)) {}

    MemRepository(std::initializer_list<Map::value_type> pairs)
        : MemRepository(std::map(pairs)) {}

    MemRepository() : MemRepository({}) {}

    auto packagesWithName(std::string name) -> ptr<Packages> override {
        if (not packageMap.contains(name)) return make<MemPackages>();
        return packageMap.at(name);
    };
};
