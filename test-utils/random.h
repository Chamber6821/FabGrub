//
// Created by Ivan on 18.02.2023.
//

#pragma once

#include <concepts>
#include <random>
#include <string>

template <std::integral T>
auto random(T min, T max) -> T {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<T> des{min, max};
    return des(gen);
}

auto randomVersionPart() -> int;

auto randomVersionPart(int min, int max) -> int;

auto randomNoSpaceName(int minLength, int maxLength) -> std::string;
