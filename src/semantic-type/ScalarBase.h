//
// Created by Ivan on 26.04.2023.
//

#pragma once

template <class T>
class ScalarBase {
    T t;

  public:
    constexpr ScalarBase() : t() {}

    constexpr explicit ScalarBase(T value) : t(value) {}

    constexpr auto value() -> T & { return t; }

    [[nodiscard]] constexpr auto value() const -> const T & { return t; }
};
