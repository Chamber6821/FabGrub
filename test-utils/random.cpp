//
// Created by Ivan on 18.02.2023.
//

#include "random.h"
#include "semver/MinVersionPart.h"
#include <random>

auto randomVersionPart() -> VersionPart {
    const auto max = Max{VersionPart{100}};
    return randomVersionPart(MinVersionPart, max);
}

auto randomVersionPart(Min<VersionPart> min, Max<VersionPart> max)
    -> VersionPart {
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_int_distribution des{min.value(), max.value()};
    return des(gen);
}

auto randomNoSpaceName() -> std::string {
    const auto min = Min{3};
    const auto max = Max{10};
    return randomNoSpaceName(min, max);
}

auto randomNoSpaceName(Min<int> minLength, Max<int> maxLength) -> std::string {
    static constexpr std::string_view alphabet = "0123456789"
                                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                                 "abcdefghijklmnopqrstuvwxyz";
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_int_distribution alpha{0ULL, alphabet.size() - 1};
    static std::uniform_int_distribution length{minLength.value(),
                                                maxLength.value()};

    std::string name("-", length(gen));
    std::generate(name.begin(), name.end(),
                  [&]() { return alphabet[alpha(gen)]; });
    return std::move(name);
}
