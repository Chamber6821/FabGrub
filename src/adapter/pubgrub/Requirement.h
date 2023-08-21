//
// Created by Ivan on 07.08.2023.
//

#pragma once

#include "Reactions/misc/owning/make.h"
#include "adapter/pubgrub/Version.h"
#include "package/Package.h"
#include "pubgrub/concepts.hpp"
#include "pubgrub/interval.hpp"
#include "semver/NextPatch.h"
#include "semver/Version.h"
#include <optional>
#include <string>
#include <utility>

namespace adapter {

class Requirement {
  public:
    using version_range_type = pubgrub::interval_set<adapter::Version>;

    // It is ugly requirement from pubgrub lib
    // NOLINTNEXTLINE(*-non-private-member-variables-in-classes)
    std::string key;
    // NOLINTNEXTLINE(*-non-private-member-variables-in-classes)
    version_range_type range;

    Requirement(std::string key, version_range_type range)
        : key(std::move(key)), range(std::move(range)) {}

    explicit Requirement(std::string key)
        : Requirement(std::move(key), version_range_type{}) {}

    Requirement(std::string key, const ptr<::Version> &version)
        : Requirement(
              std::move(key), {adapter::Version(version),
                               adapter::Version(make<NextPatch>(version))}
          ) {}

    explicit Requirement(const ptr<::Requirement> &req) : key(req->name()) {
        if (*req->low() < *req->high())
            range = {
                adapter::Version(req->low()),
                adapter::Version(req->high())};
    }

    explicit Requirement(const in<Package> &package)
        : Requirement(package->name(), package->version()) {}

    [[nodiscard]] auto with_range(version_range_type r) const noexcept
        -> Requirement {
        return {key, std::move(r)};
    }

    [[nodiscard]] auto intersection(const Requirement &other) const noexcept
        -> std::optional<Requirement> {
        const auto rng = range.intersection(other.range);
        if (rng.empty()) {
            return std::nullopt;
        }
        return with_range(rng);
    }

    [[nodiscard]] auto union_(const Requirement &other) const noexcept
        -> std::optional<Requirement> {
        const auto rng = range.union_(other.range);
        if (rng.empty()) {
            return std::nullopt;
        }
        return with_range(rng);
    }

    [[nodiscard]] auto difference(const Requirement &other) const noexcept
        -> std::optional<Requirement> {
        const auto rng = range.difference(other.range);
        if (rng.empty()) {
            return std::nullopt;
        }
        return with_range(rng);
    }

    [[nodiscard]] auto implied_by(const Requirement &other) const noexcept {
        return range.contains(other.range);
    }

    [[nodiscard]] auto excludes(const Requirement &other) const noexcept {
        return range.disjoint(other.range);
    }

    // Something copy-paste from pubgrub::test::simple_req
    // Concept don't require this, but otherwise we will have a compile error
    friend void do_repr(auto out, const Requirement *self) {
        out.type("Requirement");
        if (self) {
            out.value("{}@{}", self->key, out.repr_value(self->range));
        }
    }

    friend auto operator<<(std::ostream &out, const adapter::Requirement &r)
        -> std::ostream & {
        return out << r.key << '(' << r.range << ')';
    }
};

static_assert(pubgrub::requirement<Requirement>);

} // namespace adapter
