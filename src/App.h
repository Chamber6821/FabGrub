//
// Created by Ivan on 08.08.2023.
//

#pragma once

#include "adapter/pubgrub/Provider.h"
#include "destination/Destination.h"
#include "fmt/format.h"
#include "profile/Profiles.h"
#include "pubgrub/solve.hpp"
#include "repository/Repository.h"
#include "semver/VersionOf.h"
#include "utils/range.h"
#include <stdexcept>
#include <string>
#include <utility>

namespace pubgrub::debug {
template <class T>
try_repr(T) -> try_repr<T>;
} // namespace pubgrub::debug

class App {
    ptr<Profiles> profiles;
    ptr<Repository> repository;
    ptr<Destination> destination;

  public:
    App(ptr<Profiles> profiles, ptr<Repository> repository,
        ptr<Destination> destination)
        : profiles(std::move(profiles)), repository(std::move(repository)),
          destination(std::move(destination)) {}

    void load(std::string profile) {
        using namespace std::views;

        auto requirements =
            to_range(profiles) |
            filter([&](const auto &p) { return p->name() == profile; }) |
            transform([](const ptr<Profile> &p) { return p->requirements(); }) |
            transform(to_range<ptr<Requirement>>) | join |
            transform([](const ptr<Requirement> &req) {
                return adapter::Requirement(req);
            });

        auto solution =
            pubgrub::solve(requirements, adapter::Provider(repository));

        auto packages =
            solution | transform([&](const adapter::Requirement &req) {
                auto versionString =
                    (*req.range.iter_intervals().begin()).low.resurrect();
                auto version = VersionOf(versionString);
                auto rangeWithPackage =
                    to_range(repository->packagesWithName(req.key)) |
                    filter([&](const ptr<Package> &package) {
                        return *package->version() == version;
                    });

                if (rangeWithPackage.empty())
                    throw std::runtime_error(fmt::format(
                        "Not found package for requirement {}:{}",
                        req.key,
                        versionString
                    ));

                return *rangeWithPackage.begin();
            });

        for (ptr<Package> package : packages) {
            destination->put(std::move(package));
        }
    }
};
