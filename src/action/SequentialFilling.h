//
// Created by Ivan on 29.08.2023.
//

#pragma once

#include <utility>

#include "Action.h"
#include "destination/Destination.h"
#include "solution/Solution.h"
#include "utils/range.h"

class SequentialFilling : public Action {
    ptr<Destination> destination;
    ptr<Solution> solution;

  public:
    SequentialFilling(ptr<Destination> destination, ptr<Solution> solution)
        : destination(std::move(destination)), solution(std::move(solution)) {}

    void operator()() override {
        for (auto p : to_range(solution->packages())) {
            destination->put(std::move(p));
        }
    }
};
