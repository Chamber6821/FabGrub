//
// Created by Ivan on 18.02.2023.
//

#include "utils.h"
#include "Version.h"
#include <random>

Version getRandomVersion() {
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_int_distribution des{0, std::numeric_limits<int>::max()};

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
