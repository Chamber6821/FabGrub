#pragma once

#include "Log.h"
#include <filesystem>
#include <fstream>

class FileLog : public Log {
    std::filesystem::path outFile;

  public:
    explicit FileLog(std::filesystem::path outFile)
        : outFile(std::move(outFile)) {}

    void log(const std::string &message) override {
        auto dir = outFile;
        dir.remove_filename();
        std::filesystem::create_directories(dir);

        std::ofstream(outFile, std::ofstream::app) << message << '\n';
    }
};
