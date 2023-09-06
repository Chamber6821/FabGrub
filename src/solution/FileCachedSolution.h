//
// Created by Ivan on 31.08.2023.
//

#pragma once

#include "Reactions/misc/owning/make.h"
#include "Solution.h"
#include "package/JsonPackages.h"
#include "scalar/JsonFromFile.h"
#include "utils/range.h"

#include <utility>

class FileCachedSolution : public Solution {
    std::filesystem::file_time_type lastWriteTime;
    std::filesystem::path path;
    ptr<Solution> origin;

    static auto directory(std::filesystem::path file) {
        return file.remove_filename();
    }

    void save(const ptr<Packages> &packages) const {
        try {
            auto json = nlohmann::json::array();
            for (auto p : to_range(packages)) {
                json.push_back(nlohmann::json::array(
                    {p->name(), fmt::format("{}", fmt::streamed(*p->version()))}
                ));
            }

            std::filesystem::create_directories(directory(path));
            std::ofstream(path) << json.dump(2);
        } catch (...) {
            std::throw_with_nested(std::runtime_error(fmt::format(
                "Failed while save solution to {}",
                fmt::streamed(path)
            )));
        }
    }

  public:
    FileCachedSolution(
        std::filesystem::file_time_type lastWriteTime,
        std::filesystem::path path, ptr<Solution> origin
    )
        : lastWriteTime(lastWriteTime), path(std::move(path)),
          origin(std::move(origin)) {}

    [[nodiscard]] auto packages() const -> ptr<Packages> override {
        if (not std::filesystem::exists(path) or
            std::filesystem::last_write_time(path) < lastWriteTime)
            save(origin->packages());
        return make<JsonPackages>(make<JsonFromFile>(path));
    }
};
