//
// Created by Ivan on 29.08.2023.
//

#pragma once

#include "Log.h"
#include "utils/ptr.h"
#include <mutex>
#include <utility>

class SynchronizedLog : public Log {
    std::mutex mutex;
    ptr<Log> origin;

  public:
    explicit SynchronizedLog(ptr<Log> origin) : origin(std::move(origin)) {}

    void log(const std::string &message) override {
        const std::scoped_lock _(mutex);
        origin->log(message);
    }
};
