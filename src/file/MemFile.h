//
// Created by Ivan on 12.08.2023.
//

#pragma once

#include "File.h"
#include <fstream>
#include <utility>

class MemFile : public File {
    std::string data;

  public:
    explicit MemFile(std::string data) : data(std::move(data)) {}

    void saveTo(std::filesystem::path filepath) override {
        std::ofstream(filepath) << data;
    }
};
