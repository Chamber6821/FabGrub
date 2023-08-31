//
// Created by Ivan on 08.08.2023.
//

#pragma once

#include "Package.h"
#include "Reactions/misc/owning/make.h"
#include "file/MemFile.h"
#include "requirement/MemRequirements.h"
#include "semver/VersionOf.h"

#include <utility>

class MemPackage : public Package {
    std::string _name;
    ptr<Version> _version;
    ptr<Requirements> _requirements;

  public:
    MemPackage(
        std::string name, ptr<Version> version, ptr<Requirements> requirements
    )
        : _name(std::move(name)), _version(std::move(version)),
          _requirements(std::move(requirements)) {}

    MemPackage(std::string name, ptr<Version> version)
        : MemPackage(
              std::move(name), std::move(version), make<MemRequirements>()
          ) {}

    MemPackage(
        std::string name, const std::string &version,
        ptr<Requirements> requirements
    )
        : MemPackage(
              std::move(name), make<VersionOf>(version), std::move(requirements)
          ) {}

    MemPackage(std::string name, const std::string &version)
        : MemPackage(std::move(name), version, make<MemRequirements>()) {}

    [[nodiscard]] auto name() const -> std::string override { return _name; }

    [[nodiscard]] auto version() const -> ptr<Version> override {
        return _version;
    }

    [[nodiscard]] auto requirements() const -> ptr<Requirements> override {
        return _requirements;
    }
};
