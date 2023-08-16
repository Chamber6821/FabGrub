//
// Created by Ivan on 16.08.2023.
//

#pragma once

#include "Reactions/misc/Interface.h"
#include "fmt/core.h"

class Log : public Interface {
  public:
    virtual void log(const std::string &message) = 0;

    template <class... T>
    void info(fmt::format_string<T...> fmt, T &&...args) {
        log(fmt::format(
            std::forward<decltype(fmt)>(fmt),
            std::forward<T>(args)...
        ));
    }
};
