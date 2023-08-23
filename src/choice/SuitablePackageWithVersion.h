//
// Created by Ivan on 12.08.2023.
//

#pragma once

#include "SuitableChoice.h"
#include "fmt/format.h"
#include "fmt/ostream.h"
#include "package/Package.h"
#include "package/Packages.h"
#include "semver/Version.h"
#include <utility>

class SuitablePackageWithVersion : public SuitableChoice<ptr<Package>> {
    ptr<Version> version;

  public:
    SuitablePackageWithVersion(ptr<Packages> packages, ptr<Version> version)
        : version(std::move(version)),
          SuitableChoice(std::move(packages), [this](const ptr<Package> &p) {
              return *p->version() == *this->version;
          }) {}

    [[nodiscard]] auto element() const -> ptr<Package> override {
        try {
            return SuitableChoice::element();
        } catch (...) {
            std::throw_with_nested(std::runtime_error(fmt::format(
                "Failed while try chose package with version {}",
                fmt::streamed(*version)
            )));
        }
    }
};
