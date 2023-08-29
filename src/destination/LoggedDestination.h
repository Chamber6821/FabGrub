//
// Created by Ivan on 24.08.2023.
//

#pragma once

#include "Destination.h"
#include "fmt/ostream.h"
#include "log/Log.h"
#include <utility>

class LoggedDestination : public Destination {
    ptr<Destination> origin;
    ptr<Log> log;

  public:
    LoggedDestination(ptr<Destination> origin, ptr<Log> log)
        : origin(std::move(origin)), log(std::move(log)) {}

    void put(ptr<Package> filling) override {
        log->info("Add ", fmt::streamed(*filling));
        origin->put(std::move(filling));
    }
};
