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
            // NOLINTBEGIN(*-env33-c, *-mt-unsafe)
            auto code =
                std::system(fmt::format("{}", fmt::streamed(exe)).c_str());
            // NOLINTEND(*-env33-c, *-mt-unsafe)
            if (code != 0)
                throw std::runtime_error(fmt::format("Exit code {}", code));
        } catch (...) {
            std::throw_with_nested(std::runtime_error(
                fmt::format("Failed while run {}", fmt::streamed(exe))
            ));
        }
    }
};
