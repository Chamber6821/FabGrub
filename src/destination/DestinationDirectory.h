//
// Created by Ivan on 24.08.2023.
//

#pragma once

#include "Destination.h"
#include "file-repository/FileRepository.h"
#include "fmt/format.h"
#include "fmt/ostream.h"
#include "utils/range.h"

class DestinationDirectory : public Destination {
    std::filesystem::path dir;
    ptr<FileRepository> repo;

    static auto filename(const ptr<Package> &p) -> std::string {
        return fmt::format(
            "{}_{}.zip",
            p->name(),
            fmt::streamed(*p->version())
        );
    }

  public:
    explicit DestinationDirectory(
        std::filesystem::path dir, ptr<FileRepository> repo
    )
        : dir(std::move(dir)), repo(std::move(repo)) {}

    void fill(ptr<Packages> filling) override {
        try {
            std::filesystem::create_directories(dir);
            for (auto p : to_range(filling)) {
                repo->fileFor(p)->saveTo(dir / filename(p));
            }
        } catch (...) {
            std::throw_with_nested(std::runtime_error(fmt::format(
                "Failed while fill directory {}",
                fmt::streamed(dir)
            )));
        }
    }
};
