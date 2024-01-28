#pragma once

#include <clocale>
#include <string_view>

void set_locale(const char *locale) {
    (void)std::setlocale(LC_ALL, locale); // NOLINT(*-mt-unsafe)
}

auto get_locale() -> std::string_view {
    return std::setlocale(LC_ALL, nullptr); // NOLINT(*-mt-unsafe)
}
