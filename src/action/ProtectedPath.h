#pragma once

#include "Action.h"
#include "Reactions/misc/owning/make.h"
#include "fmt/format.h"
#include "fmt/ostream.h"
#include "log/FakeLog.h"
#include "log/Log.h"
#include "utils/defer.h"
#include "utils/ptr.h"
#include <filesystem>
#include <utility>

class ProtectedPath : public Action {
    std::filesystem::path target;
    std::filesystem::path savePath;
    ptr<Log> log;
    ptr<Action> origin;

  public:
    ProtectedPath(
        std::filesystem::path target, std::filesystem::path savePath,
        ptr<Log> log, ptr<Action> origin
    )
        : target(std::move(target)), savePath(std::move(savePath)),
          log(std::move(log)), origin(std::move(origin)) {}

    ProtectedPath(
        std::filesystem::path target, std::filesystem::path savePath,
        ptr<Action> origin
    )
        : ProtectedPath(target, savePath, make<FakeLog>(), origin) {}

    void operator()() override {
        try {
            std::filesystem::create_directories(savePath);
            std::filesystem::remove_all(savePath);
            if (std::filesystem::exists(target)) {
                log->info(
                    "Move {} to {}",
                    fmt::streamed(target),
                    fmt::streamed(savePath)
                );
                std::filesystem::rename(target, savePath);
            }
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
                if (std::filesystem::exists(savePath)) {
                    log->info(
                        "Move {} to {}",
                        fmt::streamed(savePath),
                        fmt::streamed(target)
                    );
                    std::filesystem::rename(savePath, target);
                }
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
