//
// Created by Ivan on 18.02.2023.
//

#include "random.h"
#include "semver/MinVersionPart.h"
#include <random>

auto randomVersionPart() -> VersionPart {
    const auto max = VersionPart{100};
    return randomVersionPart(MinVersionPart, max);
}

auto randomVersionPart(VersionPart min, VersionPart max) -> VersionPart {
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_int_distribution des{min, max};
    return des(gen);
}

auto randomNoSpaceName() -> std::string {
    const int minLength = 3;
    const int maxLength = 10;
    return randomNoSpaceName(minLength, maxLength);
}

auto randomNoSpaceName(int minLength, int maxLength) -> std::string {
    static constexpr std::string_view alphabet = "0123456789"
                                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                                 "abcdefghijklmnopqrstuvwxyz";
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_int_distribution alpha{0ULL, alphabet.size() - 1};
    static std::uniform_int_distribution length{minLength, maxLength};

    std::string name("-", length(gen));
    std::generate(name.begin(), name.end(),
                  [&]() { return alphabet[alpha(gen)]; });
    return std::move(name);
}
