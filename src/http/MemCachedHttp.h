//
// Created by Ivan on 23.08.2023.
//

#pragma once

#include "Http.h"
#include "utils/ptr.h"
#include <map>

class MemCachedHttp : public Http {
    ptr<Http> origin;
    mutable std::map<std::string, std::string> cache;

  public:
    explicit MemCachedHttp(ptr<Http> origin) : origin(std::move(origin)) {}

    [[nodiscard]] auto content(const std::string &url) const
        -> std::string override {
        if (not cache.contains(url)) cache[url] = origin->content(url);
        return cache[url];
    }
};
