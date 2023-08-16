//
// Created by Ivan on 16.08.2023.
//

#pragma once

#include "Log.h"
#include "utils/ptr.h"
#include <vector>

class ForkedLog : public Log {
    std::vector<ptr<Log>> logs;

  public:
    explicit ForkedLog(std::vector<ptr<Log>> logs) : logs(std::move(logs)) {}

    ForkedLog(std::initializer_list<ptr<Log>> logs)
        : ForkedLog(std::vector(logs)) {}

    template <std::convertible_to<ptr<Log>>... Args>
    explicit ForkedLog(Args... args) : ForkedLog({args...}) {}

    template <std::input_iterator It>
    ForkedLog(It begin, It end)
        : ForkedLog(std::vector<ptr<Log>>(begin, end)) {}

    template <std::ranges::input_range Range>
    explicit ForkedLog(Range range) : ForkedLog(range.begin(), range.end()) {}

    void log(const std::string &message) override {
        for (auto log : logs) {
            log->log(message);
        }
    }
};
