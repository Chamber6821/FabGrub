#pragma once

#include "Action.h"
#include "fmt/format.h"
#include "fmt/ostream.h"
#include "utils/defer.h"
#include "utils/ptr.h"
#include <filesystem>
#include <utility>

class ProtectedFolder : public Action {
    std::filesystem::path target;
    std::filesystem::path savePath;
    ptr<Action> origin;

  public:
    ProtectedFolder(
        std::filesystem::path target, std::filesystem::path savePath,
        ptr<Action> origin
    )
        : target(std::move(target)), savePath(std::move(savePath)),
          origin(std::move(origin)) {}

    void operator()() override {
        try {
            std::filesystem::create_directories(savePath);
            std::filesystem::remove_all(savePath);
            if (std::filesystem::exists(target))
                std::filesystem::rename(target, savePath);
        } catch (...) {
            std::throw_with_nested(std::runtime_error(fmt::format(
                "Failed while move {} to {}",
                fmt::streamed(target),
                fmt::streamed(savePath)
            )));
        }

        const defer finally([&] {
            try {
                std::filesystem::remove_all(target);
                if (std::filesystem::exists(savePath))
                    std::filesystem::rename(savePath, target);
            } catch (...) {
                std::throw_with_nested(std::runtime_error(fmt::format(
                    "Failed while restore {} from {}",
                    fmt::streamed(target),
                    fmt::streamed(savePath)
                )));
            }
        });

        (*origin)();
    }
};
