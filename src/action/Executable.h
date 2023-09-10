#pragma once

#include "Action.h"
#include "fmt/format.h"
#include "fmt/ostream.h"
#include <filesystem>
#include <utility>

class Executable : public Action {
    std::filesystem::path exe;

  public:
    explicit Executable(std::filesystem::path exe) : exe(std::move(exe)) {}

    void operator()() override {
        try {
            auto code = std::system(exe.string().c_str()); // NOLINT(*-env33-c)
            if (code != 0)
                throw std::runtime_error(fmt::format("Exit code {}", code));
        } catch (...) {
            std::throw_with_nested(std::runtime_error(
                fmt::format("Failed while run {}", fmt::streamed(exe))
            ));
        }
    }
};
