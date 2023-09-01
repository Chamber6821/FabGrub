//
// Created by Ivan on 16.08.2023.
//

#pragma once

#include "Log.h"
#include <ostream>
#include <utility>

class StreamLog : public Log {
    std::ostream &out; // NOLINT(*-avoid-const-or-ref-data-members)

  public:
    explicit StreamLog(std::ostream &out) : out(out) {}

    void log(const std::string &message) override { out << message << '\n'; }
};
