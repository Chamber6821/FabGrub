//
// Created by Ivan on 20.02.2023.
//

#pragma once

#include "Version.h"
#include "VersionRange.h"
#include <iosfwd>
#include <vector>

class VersionUnion {
  public:
    using container = std::vector<VersionRange>;
    using const_iterator = container::const_iterator;

    static VersionUnion nothing();

    explicit VersionUnion(const VersionRange &range);

    [[nodiscard]] bool in(const Version &version) const;
    [[nodiscard]] bool empty() const;

    VersionUnion operator||(const VersionUnion &other) const;
    VersionUnion operator&&(const VersionUnion &other) const;

    bool operator==(const VersionUnion &other) const = default;

    [[nodiscard]] const_iterator begin() const;
    [[nodiscard]] const_iterator end() const;

  private:
    std::vector<VersionRange> ranges;

    explicit VersionUnion(std::vector<VersionRange> &&ranges);
};
