//
// Created by Ivan on 23.08.2023.
//

#pragma once

#include "File.h"

class FakeFile : public File {
  public:
    void saveTo(std::filesystem::path directory) override {
        // nothing
    }
};
