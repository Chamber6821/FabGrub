//
// Created by Ivan on 18.02.2023.
//

#include "utils.h"
#include <random>
#include <ranges>

VersionPart randomVersionPart(VersionPart min, VersionPart max) {
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_int_distribution des{min, max};
    return des(gen);
}

std::string getRandomNoSpaceName() {
    static constexpr std::string_view alphabet = "0123456789"
                                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                                 "abcdefghijklmnopqrstuvwxyz";
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_int_distribution alpha{0ull, alphabet.size() - 1};
    static std::uniform_int_distribution length{3, 10};

    std::string name("-", length(gen));
    std::generate(name.begin(), name.end(),
                  [&]() { return alphabet[alpha(gen)]; });
    return std::move(name);
}
