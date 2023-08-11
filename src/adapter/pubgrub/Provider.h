//
// Created by Ivan on 08.08.2023.
//

#pragma once

#include "Requirement.h"
#include "fmt/format.h"
#include "fmt/ostream.h"
#include "pubgrub/concepts.hpp"
#include "repository/Repository.h"
#include "utils/range.h"
#include <ranges>
#include <vector>

namespace adapter {

class Provider {
    ptr<Repository> repo;

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

        auto version = (*req.range.iter_intervals().begin()).low.origin();
        auto packages = to_range(repo->packagesWithName(req.key));

        auto it = std::ranges::find_if(packages, [&](const ptr<Package> &p) {
            return p->version() == version;
        });

        if (it == packages.end())
            throw std::runtime_error(fmt::format(
                "Package with version {} not found",
                fmt::streamed(*version)
            ));

        auto package = *it;
        auto reqs = to_range(package->requirements()) |
                    transform([](const ptr<::Requirement> &req) {
                        return adapter::Requirement(req);
                    });

        return {reqs.begin(), reqs.end()};
    }
};

static_assert(pubgrub::provider<adapter::Provider, adapter::Requirement>);

} // namespace adapter
