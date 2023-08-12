//
// Created by Ivan on 11.08.2023.
//

#pragma once

#include "Solution.h"
#include "adapter/pubgrub/Provider.h"
#include "adapter/pubgrub/Requirement.h"
#include "package/MemPackages.h"
#include "package/PackageWithVersion.h"
#include "pubgrub/solve.hpp"
#include "repository/Repository.h"
#include "requirement/Requirements.h"
#include "utils/range.h"
#include <ranges>
#include <utility>

namespace pubgrub::debug {
template <class T>
try_repr(T) -> try_repr<T>;
} // namespace pubgrub::debug

class PubgrubSolution : public Solution {
    ptr<Requirements> requirements;
    ptr<Repository> repository;

    static auto origin(const adapter::Requirement &requirement) {
        return (*requirement.range.iter_intervals().begin()).low.origin();
    }

  public:
    PubgrubSolution(ptr<Requirements> requirements, ptr<Repository> repository)
        : requirements(std::move(requirements)),
          repository(std::move(repository)) {}

    [[nodiscard]] auto packages() const -> ptr<Packages> override {
        using namespace std::views;

        auto adopted = to_range(requirements) |
                       transform(constructor<adapter::Requirement>);

        auto solution = pubgrub::solve(adopted, adapter::Provider(repository));

        auto packages =
            solution | transform([&](const adapter::Requirement &req) {
                return make<PackageWithVersion>(
                    repository->packagesWithName(req.key),
                    origin(req)
                );
            });

        return make<MemPackages>(packages);
    }
};
