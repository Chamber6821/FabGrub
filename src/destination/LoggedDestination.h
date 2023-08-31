//
// Created by Ivan on 24.08.2023.
//

#pragma once

#include "Destination.h"
#include "fmt/ostream.h"
#include "log/Log.h"
#include <utility>

class LoggedDestination : public Destination {
    ptr<Log> log;
    ptr<Destination> origin;

  public:
    LoggedDestination(ptr<Log> log, ptr<Destination> origin)
        : log(std::move(log)), origin(std::move(origin)) {}

    void put(ptr<Package> filling) override {
        log->info("Add {}", fmt::streamed(*filling));
        origin->put(std::move(filling));
    }
};
