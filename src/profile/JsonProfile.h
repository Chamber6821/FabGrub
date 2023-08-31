//
// Created by Ivan on 31.08.2023.
//

#pragma once

#include "Profile.h"
#include "Reactions/misc/Scalar.h"
#include "Reactions/misc/owning/make.h"
#include "nlohmann/json.hpp"
#include "re146/JsonReqirements.h"
#include "semver/VersionOf.h"

class JsonProfile : public Profile {
    ptr<Scalar<nlohmann::json>> json;

  public:
    explicit JsonProfile(ptr<Scalar<nlohmann::json>> json)
        : json(std::move(json)) {}

    auto factorioVersion() const -> ptr<Version> override {
        try {
            return make<VersionOf>(json->value()["factorio"].get<std::string>()
            );
        } catch (...) {
            std::throw_with_nested(std::runtime_error(
                "Failed while get factorio version from JSON profile"
            ));
        }
    }

    auto requirements() const -> ptr<Requirements> override {
        try {
            return make<re146::JsonRequirements>(json->value()["requirements"]);
        } catch (...) {
            std::throw_with_nested(std::runtime_error(
                "Failed while get requirements from JSON profile"
            ));
        }
    }
};
