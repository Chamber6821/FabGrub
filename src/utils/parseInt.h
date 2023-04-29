//
// Created by Ivan on 27.04.2023.
//

#pragma once

#include <stdexcept>
#include <string_view>

constexpr auto isDigit(char c) -> bool { return '0' <= c and c <= '9'; }

constexpr auto digit(char c) -> int {
    if (not isDigit(c)) throw std::invalid_argument("Got not a digit");
    return c - '0';
}

namespace details {
// NOLINTNEXTLINE(misc-no-recursion)
constexpr auto parseIntImpl(std::string_view str, int value) -> int {
    const auto base = 10;

    if (str.empty()) return value;
    return parseIntImpl(str.substr(1), (value * base) + digit(str.front()));
}
} // namespace details

constexpr auto parseIntWithoutSign(std::string_view str) -> int {
    using details::parseIntImpl;
    if (str.empty()) throw std::invalid_argument("Empty string isn't a number");
    return parseIntImpl(str, 0);
}

constexpr auto parseInt(std::string_view str) -> int {
    if (str.empty()) throw std::invalid_argument("Empty string isn't a number");
    if ((str.starts_with('-') or str.starts_with('+')) and str.size() < 2)
        throw std::invalid_argument(
            "String started with sign, but has no digits");

    if (str.starts_with('-')) return -parseIntWithoutSign(str.substr(1));
    if (str.starts_with('+')) return parseIntWithoutSign(str.substr(1));
    return parseIntWithoutSign(str);
}
