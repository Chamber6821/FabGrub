#pragma once

#include "Log.h"

class FakeLog : public Log {
  public:
    void log(const std::string &message) override {
        // nothing
    }
};
