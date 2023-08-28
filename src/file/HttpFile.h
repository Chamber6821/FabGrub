//
// Created by Ivan on 28.08.2023.
//

#pragma once

#include "File.h"
#include "fmt/format.h"
#include "fmt/ostream.h"
#include "http/Http.h"
#include "utils/ptr.h"
#include <fstream>

class HttpFile : public File {
    std::string url;
    ptr<Http> http;

  public:
    HttpFile(std::string url, ptr<Http> http)
        : url(std::move(url)), http(std::move(http)) {}

    void saveTo(std::filesystem::path filepath) override {
        try {
            std::ofstream(filepath, std::ofstream::binary)
                << http->content(url);
        } catch (...) {
            std::throw_with_nested(std::runtime_error(fmt::format(
                "Failed while save file to {} from {}",
                fmt::streamed(filepath),
                url
            )));
        }
    }
};
