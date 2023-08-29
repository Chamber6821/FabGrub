//
// Created by Ivan on 24.08.2023.
//

#pragma once

#include "Destination.h"
#include "file-repository/FileRepository.h"
#include "fmt/format.h"
#include "fmt/ostream.h"

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

    void put(ptr<Package> filling) override {
        try {
            std::filesystem::create_directories(dir);
            repo->fileFor(filling)->saveTo(dir / filename(filling));
        } catch (...) {
            std::throw_with_nested(std::runtime_error(fmt::format(
                "Failed while put file for package {} to {}",
                fmt::streamed(*filling),
                fmt::streamed(dir)
            )));
        }
    }
};
