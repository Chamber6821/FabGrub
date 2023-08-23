//
// Created by Ivan on 16.08.2023.
//

#pragma once

#include "Log.h"
#include "utils/ptr.h"
#include <ostream>
#include <utility>

class StreamLog : public Log {
    ptr<std::ostream> out;

  public:
    explicit StreamLog(ptr<std::ostream> out) : out(std::move(out)) {}

    void log(const std::string &message) override { *out << message << '\n'; }
};
