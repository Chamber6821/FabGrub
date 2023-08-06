//
// Created by Ivan on 18.02.2023.
//

#include "random.h"
#include <algorithm>
#include <utility>

auto randomVersionPart() -> int { return randomVersionPart(0, 100); }

auto randomVersionPart(int min, int max) -> int { return random(min, max); }

auto randomNoSpaceName(int minLength, int maxLength) -> std::string {
    static constexpr std::string_view alphabet = "0123456789"
                                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                                 "abcdefghijklmnopqrstuvwxyz";

    const auto alpha = [&]() {
        return alphabet[random(std::size_t{0}, alphabet.size() - 1)];
    };
    const auto length = [&]() { return random(minLength, maxLength); };

    std::string name("-", length());
    std::generate(name.begin(), name.end(), alpha);
    return std::move(name);
}
