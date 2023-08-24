//
// Created by Ivan on 16.08.2023.
//

#pragma once

#include "file/File.h"
#include "fmt/format.h"
#include "fmt/ostream.h"
#include "http/Http.h"
#include "semver/Version.h"
#include "utils/ptr.h"
#include <utility>

namespace re146 {

class HttpFile : public File {
    std::string name;
    ptr<Version> version;
    ptr<Http> http;

  public:
    HttpFile(std::string name, ptr<Version> version, ptr<Http> http)
        : name(std::move(name)), version(std::move(version)),
          http(std::move(http)) {}

    void saveTo(std::filesystem::path directory) override {
        try {
            const auto filename =
                fmt::format("{}_{}.zip", name, fmt::streamed(*version));
            const auto url = fmt::format(
                "https://mods-storage.re146.dev/{}/{}.zip",
                name,
                fmt::streamed(*version)
            );

            std::ofstream(directory / filename) << http->content(url);
        } catch (...) {
            std::throw_with_nested(std::runtime_error(fmt::format(
                "Failed save {}:{} to {}",
                name,
                fmt::streamed(*version),
                directory.string()
            )));
        }
    }
};

} // namespace re146