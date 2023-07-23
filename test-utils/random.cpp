//
// Created by Ivan on 18.02.2023.
//

#include "random.h"
#include "semver/MinVersionPart.h"
#include <algorithm>

auto randomVersionPart() -> VersionPart {
    const auto max = Max{VersionPart{100}};
    return randomVersionPart(MinVersionPart, max);
}

auto randomVersionPart(Min<VersionPart> min, Max<VersionPart> max)
    -> VersionPart {
    return random(min, max);
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

    const auto alpha = [&]() {
        return alphabet[random(Min{std::size_t{0}}, Max{alphabet.size() - 1})];
    };
    const auto length = [&]() { return random(minLength, maxLength); };

    std::string name("-", length());
    std::generate(name.begin(), name.end(), alpha);
    return std::move(name);
}
