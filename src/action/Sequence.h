#pragma once

#include "Action.h"
#include "utils/ptr.h"
#include <vector>

class Sequence : public Action {
    std::vector<ptr<Action>> seq;

  public:
    explicit Sequence(std::vector<ptr<Action>> seq) : seq(std::move(seq)) {}

    Sequence(std::initializer_list<ptr<Action>> seq)
        : Sequence(std::vector(seq)) {}

    template <std::convertible_to<ptr<Action>>... Args>
    explicit Sequence(Args... args) : Sequence({args...}) {}

    template <std::input_iterator It>
    Sequence(It begin, It end)
        : Sequence(std::vector<ptr<Action>>(begin, end)) {}

    template <std::ranges::input_range Range>
    explicit Sequence(Range range) : Sequence(range.begin(), range.end()) {}

    void operator()() override {
        for (auto &a : seq) {
            (*a)();
        }
    }
};
