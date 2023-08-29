//
// Created by Ivan on 28.08.2023.
//

#pragma once

#include "FileRepository.h"
#include "Reactions/misc/owning/make.h"
#include <utility>

class FileCachedFileRepository : public FileRepository {
    std::filesystem::path dir;
    ptr<FileRepository> origin;

    class CachedFile : public File {
        std::filesystem::path dir;
        ptr<File> origin;

      public:
        CachedFile(std::filesystem::path dir, ptr<File> origin)
            : dir(std::move(dir)), origin(std::move(origin)) {}

        void saveTo(std::filesystem::path filepath) override {
            auto cached = dir / filepath.filename();
            try {
                std::filesystem::create_directories(dir);
                if (not std::filesystem::exists(cached)) origin->saveTo(cached);
                if (not std::filesystem::exists(filepath))
                    std::filesystem::create_hard_link(cached, filepath);
            } catch (...) {
                std::throw_with_nested(std::runtime_error(fmt::format(
                    "Failed while save cached file {}",
                    fmt::streamed(cached)
                )));
            }
        }
    };

  public:
    FileCachedFileRepository(
        std::filesystem::path dir, ptr<FileRepository> origin
    )
        : dir(std::move(dir)), origin(std::move(origin)) {}

    auto fileFor(ptr<Package> package) -> ptr<File> override {
        return make<CachedFile>(dir, origin->fileFor(package));
    }
};
