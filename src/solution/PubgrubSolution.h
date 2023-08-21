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

    class ExplainHandler {
        std::stringstream message;

        void say(pubgrub::explain::dependency<adapter::Requirement> dep) {
            message << dep.dependent << " requires " << dep.dependency;
        }

        void say(pubgrub::explain::disallowed<adapter::Requirement> dep) {
            message << dep.requirement << " is not allowed";
        }

        void say(pubgrub::explain::unavailable<adapter::Requirement> un) {
            message << un.requirement << " is not available";
        }

        void say(pubgrub::explain::needed<adapter::Requirement> need) {
            message << need.requirement << " is needed";
        }

        void say(pubgrub::explain::conflict<adapter::Requirement> conf) {
            message << conf.a << " conflicts with " << conf.b;
        }

        void say(pubgrub::explain::compromise<adapter::Requirement> comp) {
            message << comp.left << " and " << comp.right << " agree on "
                    << comp.result;
        }

        void say(pubgrub::explain::no_solution /*unused*/) {
            message << "There is no solution";
        }

      public:
        [[nodiscard]] auto view() const -> std::string_view {
            return message.view();
        }

        void operator()(pubgrub::explain::separator /*unused*/) {
            message << '\n';
        }

        template <typename What>
        void operator()(pubgrub::explain::conclusion<What> c) {
            message << "Thus: ";
            say(c.value);
            message << '\n';
        }

        template <typename What>
        void operator()(pubgrub::explain::premise<What> c) {
            message << "Known: ";
            say(c.value);
            message << '\n';
        }
    };

    static auto origin(const adapter::Requirement &requirement) {
        return (*requirement.range.iter_intervals().begin()).low.origin();
    }

  public:
    PubgrubSolution(ptr<Requirements> requirements, ptr<Repository> repository)
        : requirements(std::move(requirements)),
          repository(std::move(repository)) {}

    [[nodiscard]] auto packages() const -> ptr<Packages> override {
        try {
            using namespace std::views;

            auto adopted = to_range(requirements) |
                           transform(constructor<adapter::Requirement>);

            try {
                auto solution =
                    pubgrub::solve(adopted, adapter::Provider(repository));

                auto packages =
                    solution | transform([&](const adapter::Requirement &req) {
                        return make<PackageWithVersion>(
                            repository->packagesWithName(req.key),
                            origin(req)
                        );
                    });

                return make<MemPackages>(packages);
            } catch (
                const pubgrub::solve_failure_type_t<adapter::Requirement> &e
            ) {
                ExplainHandler ex;
                pubgrub::generate_explaination(e, ex);
                throw std::runtime_error(std::string(ex.view()));
            }
        } catch (...) {
            std::throw_with_nested(
                std::runtime_error("Failed while solve using PubGrub")
            );
        }
    }
};
