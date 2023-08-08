//
// Created by Ivan on 07.08.2023.
//

#pragma once

#include "Requirements.h"
#include <utility>
#include <vector>

class MemRequirements : public Requirements {
    std::vector<ptr<Requirement>> reqs;

  public:
    explicit MemRequirements(std::vector<ptr<Requirement>> reqs)
        : reqs(std::move(reqs)) {}

    MemRequirements(std::initializer_list<ptr<Requirement>> reqs)
        : MemRequirements(std::vector(reqs)) {}

    template <std::convertible_to<ptr<Requirement>>... Args>
    explicit MemRequirements(Args... args)
        : MemRequirements(std::initializer_list(args...)) {}

    auto count() -> int override { return static_cast<int>(reqs.size()); };

    auto at(int index) -> ptr<Requirement> override { return reqs.at(index); };
};
