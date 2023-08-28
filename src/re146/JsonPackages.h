//
// Created by Ivan on 16.08.2023.
//

#pragma once

#include "JsonPackage.h"
#include "Reactions/misc/owning/make.h"
#include "nlohmann/json.hpp"
#include "package/Packages.h"
#include <utility>

namespace re146 {

class JsonPackages : public Packages {
    nlohmann::json json;

  public:
    explicit JsonPackages(nlohmann::json json) : json(std::move(json)) {}

    explicit JsonPackages(const std::string &json)
        : JsonPackages(nlohmann::json::parse(json)) {}

    [[nodiscard]] auto count() const -> int override {
        return static_cast<int>(json["releases"].size());
    }

    [[nodiscard]] auto at(int index) const -> ptr<Package> override {
        return make<JsonPackage>(json["name"], json["releases"][index]);
    }
};

} // namespace re146