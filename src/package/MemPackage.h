//
// Created by Ivan on 08.08.2023.
//

#pragma once

#include "Package.h"
#include "Reactions/misc/owning/make.h"
#include "requirement/MemRequirements.h"
#include "semver/VersionOf.h"

#include <utility>

class MemPackage : public Package {
    std::string _name;
    ptr<Version> _version;
    ptr<Requirements> _requirements;
    std::string _data;

  public:
    MemPackage(
        std::string name, ptr<Version> version, ptr<Requirements> requirements,
        std::string data
    )
        : _name(std::move(name)), _version(std::move(version)),
          _requirements(std::move(requirements)), _data(std::move(data)) {}

    MemPackage(
        std::string name, const std::string &version,
        ptr<Requirements> requirements, std::string data
    )
        : MemPackage(
              std::move(name), make<VersionOf>(version),
              std::move(requirements), std::move(data)
          ) {}

    MemPackage(std::string name, const std::string &version, std::string data)
        : MemPackage(
              std::move(name), version, make<MemRequirements>(), std::move(data)
          ) {}

    [[nodiscard]] auto name() const -> std::string override { return _name; }

    [[nodiscard]] auto version() const -> ptr<Version> override {
        return _version;
    }

    [[nodiscard]] auto requirements() const -> ptr<Requirements> override {
        return _requirements;
    }

    [[nodiscard]] auto data() const -> std::string override { return _data; }
};
