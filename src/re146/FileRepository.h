//
// Created by Ivan on 28.08.2023.
//

#pragma once

#include "Reactions/misc/owning/make.h"
#include "file-repository/FileRepository.h"
#include "file/HttpFile.h"
#include "http/Http.h"
#include "utils/urlEncoded.h"

namespace re146 {

class FileRepository : public ::FileRepository {
    ptr<Http> http;

  public:
    explicit FileRepository(ptr<Http> http) : http(std::move(http)) {}

    [[nodiscard]] auto fileFor(ptr<Package> package) -> ptr<File> override {
        return make<HttpFile>(
            fmt::format(
                "https://mods-storage.re146.dev/{}/{}.zip",
                urlEncoded(package->name()),
                fmt::streamed(*package->version())
            ),
            http
        );
    }
};

} // namespace re146
