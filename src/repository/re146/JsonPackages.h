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
    ptr<Http> http;

  public:
    explicit JsonPackages(nlohmann::json json, ptr<Http> http)
        : json(std::move(json)), http(std::move(http)) {}

    explicit JsonPackages(const std::string &json, ptr<Http> http)
        : JsonPackages(nlohmann::json::parse(json), std::move(http)) {}

    [[nodiscard]] auto count() const -> int override {
        return static_cast<int>(json["releases"].size());
    }

    [[nodiscard]] auto at(int index) const -> ptr<Package> override {
        return make<JsonPackage>(json["name"], json["releases"][index], http);
    }
};

} // namespace re146