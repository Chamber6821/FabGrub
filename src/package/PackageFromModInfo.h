#pragma once

#include "Package.h"
#include "Reactions/misc/Scalar.h"
#include "Reactions/misc/owning/make.h"
#include "nlohmann/json.hpp"
#include "re146/JsonReqirements.h"
#include "scalar/JsonFromFile.h"
#include "semver/VersionOf.h"

class PackageFromModInfo : public Package {
    ptr<Scalar<nlohmann::json>> json;

  public:
    explicit PackageFromModInfo(ptr<Scalar<nlohmann::json>> json)
        : json(std::move(json)) {}

    explicit PackageFromModInfo(const std::filesystem::path &file)
        : PackageFromModInfo(make<JsonFromFile>(file)) {}

    [[nodiscard]] auto name() const -> std::string override {
        return json->value()["name"].get<std::string>();
    }

    [[nodiscard]] auto version() const -> ptr<Version> override {
        return make<VersionOf>(json->value()["version"].get<std::string>());
    }

    [[nodiscard]] auto requirements() const -> ptr<Requirements> override {
        return make<re146::JsonRequirements>(json->value()["dependencies"]);
    }
};
