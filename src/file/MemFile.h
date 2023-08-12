//
// Created by Ivan on 12.08.2023.
//

#pragma once

#include "File.h"
#include <fstream>

class MemFile : public File {
    std::string name;
    std::string data;

  public:
    MemFile(std::string name, std::string data) : name(name), data(data) {}

    void saveTo(std::filesystem::path directory) override {
        std::ofstream(directory / name) << data;
    }
};
