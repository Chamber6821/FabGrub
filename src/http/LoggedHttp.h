//
// Created by Ivan on 16.08.2023.
//

#pragma once

#include "Http.h"
#include "log/Log.h"
#include "utils/ptr.h"

class LoggedHttp : public Http {
    ptr<Log> log;
    ptr<Http> origin;

  public:
    LoggedHttp(ptr<Log> log, ptr<Http> http)
        : log(std::move(log)), origin(std::move(http)) {}

    [[nodiscard]] auto content(const std::string &url) const
        -> std::string override {
        log->info("Get: {}", url);
        return origin->content(url);
    }
};
