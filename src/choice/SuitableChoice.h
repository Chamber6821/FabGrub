//
// Created by Ivan on 12.08.2023.
//

#pragma once

#include "Choice.h"
#include "utils/List.h"
#include "utils/ptr.h"
#include "utils/range.h"
#include <functional>
#include <stdexcept>

template <class T>
class SuitableChoice : public Choice<T> {
    ptr<List<T>> elements;
    std::function<bool(const T &)> predicate;

  public:
    SuitableChoice(
        ptr<List<T>> elements, std::function<bool(const T &)> predicate
    )
        : elements(std::move(elements)), predicate(std::move(predicate)) {}

    [[nodiscard]] auto element() const -> T override {
        try {
            auto range = to_range(elements);
            auto result = std::ranges::find_if(range, predicate);
            if (result == range.end())
                throw std::runtime_error("Suitable element does not exist");
            return *result;
        } catch (...) {
            std::throw_with_nested(
                std::runtime_error("Failed while choose element")
            );
        }
    }
};
