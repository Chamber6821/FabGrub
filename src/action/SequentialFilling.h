//
// Created by Ivan on 29.08.2023.
//

#pragma once

#include <utility>

#include "Action.h"
#include "cancellation/Cancellation.h"
#include "destination/Destination.h"
#include "solution/Solution.h"
#include "utils/range.h"

class SequentialFilling : public Action {
    ptr<Cancellation> cancellation;
    ptr<Destination> destination;
    ptr<Solution> solution;

  public:
    SequentialFilling(
        ptr<Cancellation> cancellation, ptr<Destination> destination,
        ptr<Solution> solution
    )
        : cancellation(std::move(cancellation)),
          destination(std::move(destination)), solution(std::move(solution)) {}

    void operator()() override {
        for (auto p : to_range(solution->packages())) {
            cancellation->throwIfCanceled();
            destination->put(std::move(p));
        }
    }
};
