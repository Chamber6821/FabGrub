//
// Created by Ivan on 16.08.2023.
//

#pragma once

#include "HttpFile.h"
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
    ptr<Http> http;

  public:
    JsonPackage(std::string name, nlohmann::json json, ptr<Http> http)
        : _name(std::move(name)), json(std::move(json)), http(std::move(http)) {
    }

    [[nodiscard]] auto name() const -> std::string override { return _name; }

    [[nodiscard]] auto version() const -> ptr<Version> override {
        return make<VersionOf>(json["version"]);
    }

    [[nodiscard]] auto requirements() const -> ptr<Requirements> override {
        return make<JsonRequirements>(json["info_json"]["dependencies"]);
    }

    [[nodiscard]] auto file() const -> ptr<File> override {
        return make<HttpFile>(name(), version(), http);
    }
};

} // namespace re146