#include "urlEncoded.h"
#include "curl/curl.h"
#include "defer.h"

auto urlEncoded(const std::string &urlPart) -> std::string {
    const auto encoded_value = curl_easy_escape(
        nullptr,
        urlPart.c_str(),
        static_cast<int>(urlPart.length())
    );
    defer finally([&] { curl_free(encoded_value); });
    return encoded_value;
}
