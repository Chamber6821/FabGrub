#pragma once

#include <exception>
#include <string>

// NOLINTNEXTLINE(*-no-recursion)
auto stringify(const std::exception &e, int level) -> std::string {
    auto part = std::string(level, ' ') + "exception: " + e.what() + '\n';
    try {
        std::rethrow_if_nested(e);
        return part;
    } catch (const std::exception &nestedException) {
        return part + stringify(nestedException, level + 1);
    }
}
