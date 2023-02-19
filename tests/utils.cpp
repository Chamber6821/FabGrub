//
// Created by Ivan on 18.02.2023.
//

#include "utils.h"
#include <random>
#include <ranges>

Version getRandomVersion() {
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_int_distribution des{0, 1000};

    return {.major = des(gen), .minor = des(gen), .patch = des(gen)};
}

Version getRandomVersionLessThan(Version barrier) {
    auto result = getRandomVersion();
    while (result >= barrier)
        result = getRandomVersion();
    return result;
}

Version getRandomVersionGreaterThan(Version barrier) {
    auto result = getRandomVersion();
    while (result <= barrier)
        result = getRandomVersion();
    return result;
}

Version getRandomVersionBetween(Version left, Version right) {
    auto result = getRandomVersion();
    while (!(left < result && result < right))
        result = getRandomVersion();
    return result;
}

VersionRange getRandomRange() {
    return {getRandomVersion(), getRandomVersion()};
}

VersionRange getRandomNotEmptyRange() {
    auto left = getRandomVersion();
    auto right = getRandomVersionGreaterThan(left);
    return {left, right};
}

VersionRange getRandomEmptyRange() {
    auto left = getRandomVersion();
    auto right = getRandomVersionLessThan(left);
    return {left, right};
}

std::string getRandomNoSpaceName() {
    static constexpr std::string_view alphabet = "0123456789"
                                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                                 "abcdefghijklmnopqrstuvwxyz";
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_int_distribution alpha{0ull, alphabet.size() - 1};
    static std::uniform_int_distribution length{3, 10};

    std::string name("-", length(gen));
    std::generate(name.begin(), name.end(), [&]() { return alphabet[alpha(gen)]; });
    return std::move(name);
}
