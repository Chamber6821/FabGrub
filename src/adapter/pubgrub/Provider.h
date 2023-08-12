//
// Created by Ivan on 08.08.2023.
//

#pragma once

#include "Requirement.h"
#include "fmt/format.h"
#include "fmt/ostream.h"
#include "package/ChosenPackage.h"
#include "package/PackageWithVersion.h"
#include "pubgrub/concepts.hpp"
#include "repository/Repository.h"
#include "utils/range.h"
#include <ranges>
#include <vector>

namespace adapter {

class Provider {
    ptr<Repository> repo;

    static auto origin(const adapter::Requirement &requirement) {
        return (*requirement.range.iter_intervals().begin()).low.origin();
    }

  public:
    explicit Provider(ptr<Repository> repo) : repo(std::move(repo)) {}

    [[nodiscard]] auto best_candidate(const adapter::Requirement &req) const
        -> std::optional<adapter::Requirement> {
        using namespace std::views;

        auto packagesInRange =
            to_range(repo->packagesWithName(req.key)) |
            filter([&](const ptr<Package> &package) {
                return req.range.contains(adapter::Version(package->version()));
            });

        if (packagesInRange.empty()) return std::nullopt;

        auto bestPackage =
            std::ranges::max(packagesInRange, [](const auto &l, const auto &r) {
                return *l->version() < *r->version();
            });
        return Requirement{
            bestPackage->name(),
            {adapter::Version(bestPackage->version()),
             adapter::Version(make<NextPatch>(bestPackage->version()))}};
    }

    [[nodiscard]] auto requirements_of(const adapter::Requirement &req) const
        -> std::vector<adapter::Requirement> {
        using namespace std::views;

        auto package = make<PackageWithVersion>(
            repo->packagesWithName(req.key),
            origin(req)
        );

        auto reqs = to_range(package->requirements()) |
                    transform([](const ptr<::Requirement> &req) {
                        return adapter::Requirement(req);
                    });

        return {reqs.begin(), reqs.end()};
    }
};

static_assert(pubgrub::provider<adapter::Provider, adapter::Requirement>);

} // namespace adapter
