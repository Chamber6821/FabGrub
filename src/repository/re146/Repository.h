//
// Created by Ivan on 16.08.2023.
//

#pragma once

#include "JsonPackages.h"
#include "Reactions/misc/owning/make.h"
#include "http/Http.h"
#include "repository/Repository.h"

namespace re146 {

class Repository : public ::Repository {
    ptr<Http> http;

  public:
    explicit Repository(ptr<Http> http) : http(std::move(http)) {}

    [[nodiscard]] auto packagesWithName(std::string name) const
        -> ptr<Packages> override {
        const auto url = "https://re146.dev/factorio/mods/modinfo?id=" + name;
        return make<JsonPackages>(http->content(url));
    }
};

} // namespace re146