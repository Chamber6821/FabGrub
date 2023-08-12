//
// Created by Ivan on 12.08.2023.
//

#pragma once

#include "Choice.h"
#include "utils/ptr.h"
#include <optional>

template <class T>
class CachedChoice : public Choice<T> {
    ptr<Choice<T>> choice;
    mutable std::optional<T> cached = std::nullopt;

  public:
    explicit CachedChoice(ptr<Choice<T>> choice) : choice(std::move(choice)) {}

    [[nodiscard]] auto element() const -> T override {
        if (not cached.has_value()) cached = choice->element();
        return cached.value();
    }
};
