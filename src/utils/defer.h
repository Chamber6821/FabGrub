#pragma once

#include <functional>

class defer {
    std::function<void()> func;

  public:
    explicit defer(std::function<void()> func) : func(std::move(func)) {}

    ~defer() noexcept(false) { func(); }
};
