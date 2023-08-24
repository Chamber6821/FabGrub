//
// Created by Ivan on 24.08.2023.
//

#pragma once

#include "Destination.h"
#include "fmt/ostream.h"
#include "log/Log.h"
#include "utils/range.h"
#include <utility>

class LoggedDestination : public Destination {
    ptr<Destination> origin;
    ptr<Log> log;

  public:
    LoggedDestination(ptr<Destination> origin, ptr<Log> log)
        : origin(std::move(origin)), log(std::move(log)) {}

    void fill(ptr<Packages> filling) override {
        for (auto p : to_range(filling)) {
            log->info(
                "Chosen {}:{}",
                p->name(),
                fmt::streamed(*p->version())
            );
        }
        origin->fill(std::move(filling));
    }
};
