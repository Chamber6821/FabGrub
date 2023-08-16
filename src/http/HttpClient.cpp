//
// Created by Ivan on 16.08.2023.
//
#include "HttpClient.h"
#include "fmt/format.h"

#include <cpr/cpr.h>
#include <cpr/response.h>

#include <stdexcept>

auto HttpClient::content(const std::string &url) const -> std::string {
    try {
        const auto CLIENT_ERROR_MIN = 400;
        const auto SERVER_ERROR_MAX = 599;

        auto response = cpr::Get(cpr::Url(url));
        auto status = response.status_code;
        if (CLIENT_ERROR_MIN <= status && status <= SERVER_ERROR_MAX)
            throw std::runtime_error(fmt::format("Status code {}", status));
        return response.text;
    } catch (...) {
        std::throw_with_nested(std::runtime_error(
            fmt::format("Failed while make GET request to {}", url)
        ));
    }
}
