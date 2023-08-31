#pragma once

#include "MemPackage.h"
#include "Packages.h"

#include "Reactions/misc/Scalar.h"
#include "Reactions/misc/owning/make.h"
#include "nlohmann/json.hpp"
#include <utility>

class JsonPackages : public Packages {
    ptr<Scalar<nlohmann::json>> json;

  public:
    explicit JsonPackages(ptr<Scalar<nlohmann::json>> json)
        : json(std::move(json)) {}

    [[nodiscard]] auto count() const -> int override {
        return static_cast<int>(json->value().size());
    }

    [[nodiscard]] auto at(int index) const -> ptr<Package> override {
        return make<MemPackage>(
            json->value()[index][0].get<std::string>(),
            json->value()[index][1].get<std::string>()
        );
    }
};
