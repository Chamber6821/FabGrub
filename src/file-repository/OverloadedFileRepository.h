//
// Created by Ivan on 28.08.2023.
//

#pragma once

#include "FileRepository.h"
#include <utility>

class OverloadedFileRepository : public FileRepository {
    ptr<Package> overloaded;
    ptr<File> overloading;
    ptr<FileRepository> origin;

  public:
    OverloadedFileRepository(
        ptr<Package> overloaded, ptr<File> overloading,
        ptr<FileRepository> origin
    )
        : overloaded(std::move(overloaded)),
          overloading(std::move(overloading)), origin(std::move(origin)) {}

    auto fileFor(ptr<Package> package) -> ptr<File> override {
        if (*package == *overloaded) return overloading;
        return origin->fileFor(std::move(package));
    }
};
