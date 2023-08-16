//
// Created by Ivan on 16.08.2023.
//

#pragma once

#include "Http.h"
#include "log/Log.h"
#include "utils/ptr.h"

class LoggedHttp : public Http {
    ptr<Http> http;
    ptr<Log> log;

  public:
    LoggedHttp(ptr<Http> http, ptr<Log> log)
        : http(std::move(http)), log(std::move(log)) {}

    [[nodiscard]] auto content(const std::string &url) const -> std::string override {
        log->info("Get: {}", url);
        return http->content(url);
    }
};
