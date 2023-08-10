//
// Created by Ivan on 08.08.2023.
//

#pragma once

#include "Requirement.h"
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
                return adapter::Version(l->version()) <
                       adapter::Version(r->version());
            });
        return Requirement{
            bestPackage->name(),
            {adapter::Version(bestPackage->version()),
             adapter::Version(make<NextPatch>(bestPackage->version()))}};
    }

    [[nodiscard]] auto requirements_of(const adapter::Requirement &req) const
        -> std::vector<adapter::Requirement> {
        using namespace std::views;

        auto version = (*req.range.iter_intervals().begin()).low;
        auto packages = to_range(repo->packagesWithName(req.key));

        for (auto package : packages) {
            if (package->version() == version) {
                auto reqs = to_range(package->requirements()) |
                            transform([](const ptr<::Requirement> &req) {
                                return adapter::Requirement(req);
                            });
                std::vector<adapter::Requirement> result;
                std::ranges::copy(reqs, std::back_inserter(result));
                return result;
            }
        }

        assert(false && "Impossible?");
        std::terminate();
    }
};

static_assert(pubgrub::provider<adapter::Provider, adapter::Requirement>);

} // namespace adapter
