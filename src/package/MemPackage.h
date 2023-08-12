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
    ptr<File> _file;

  public:
    MemPackage(
        std::string name, ptr<Version> version, ptr<Requirements> requirements,
        ptr<File> file
    )
        : _name(std::move(name)), _version(std::move(version)),
          _requirements(std::move(requirements)), _file(std::move(file)) {}

    MemPackage(
        std::string name, const std::string &version,
        ptr<Requirements> requirements, ptr<File> file
    )
        : MemPackage(
              std::move(name), make<VersionOf>(version),
              std::move(requirements), std::move(file)
          ) {}

    MemPackage(std::string name, const std::string &version, ptr<File> file)
        : MemPackage(
              std::move(name), version, make<MemRequirements>(), std::move(file)
          ) {}

    MemPackage(
        const std::string &name, const std::string &version, std::string data
    )
        : MemPackage(name, version, make<MemFile>(name, std::move(data))) {}

    [[nodiscard]] auto name() const -> std::string override { return _name; }

    [[nodiscard]] auto version() const -> ptr<Version> override {
        return _version;
    }

    [[nodiscard]] auto requirements() const -> ptr<Requirements> override {
        return _requirements;
    }

    [[nodiscard]] auto file() const -> ptr<File> override { return _file; }
};
