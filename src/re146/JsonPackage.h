//
// Created by Ivan on 16.08.2023.
//

#pragma once

#include "JsonReqirements.h"
#include "Reactions/misc/owning/make.h"
#include "nlohmann/json.hpp"
#include "package/Package.h"
#include "semver/VersionOf.h"
#include <utility>

namespace re146 {

class JsonPackage : public Package {
    std::string _name;
    nlohmann::json json;

  public:
    JsonPackage(std::string name, nlohmann::json json)
        : _name(std::move(name)), json(std::move(json)) {}

    [[nodiscard]] auto name() const -> std::string override { return _name; }

    [[nodiscard]] auto version() const -> ptr<Version> override {
        return make<VersionOf>(json["version"]);
    }

    [[nodiscard]] auto requirements() const -> ptr<Requirements> override {
        return make<JsonRequirements>(json["info_json"]["dependencies"]);
    }
};

} // namespace re146