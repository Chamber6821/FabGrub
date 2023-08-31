//
// Created by Ivan on 31.08.2023.
//

#pragma once

#include "Reactions/misc/Scalar.h"
#include "fmt/format.h"
#include "fmt/ostream.h"
#include "nlohmann/json.hpp"
#include <fstream>

class JsonFromFile : public Scalar<nlohmann::json> {
    std::filesystem::path path;

  public:
    explicit JsonFromFile(std::filesystem::path path) : path(std::move(path)) {}

    [[nodiscard]] auto value() -> nlohmann::json override {
        try {
            if (not std::filesystem::exists(path))
                throw std::runtime_error(
                    fmt::format("File {} not exists", fmt::streamed(path))
                );
            return nlohmann::json::parse(std::ifstream(path));
        } catch (...) {
            std::throw_with_nested(std::runtime_error(fmt::format(
                "Failed while parse JSON from {}",
                fmt::streamed(path)
            )));
        }
    }
};
