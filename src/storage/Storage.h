//
// Created by Ivan on 26.01.2023.
//

#pragma once

#include "ReadOnlyStorage.h"
#include <filesystem>

class Storage : public ReadOnlyStorage {
  public:
    class Archive : public ReadOnlyArchive {
      public:
        virtual bool andRemove() = 0;
    };

    virtual std::unique_ptr<Archive> findFor(const ModImplementation &mod) = 0;

    virtual std::filesystem::path folder() = 0;
    virtual void clear() = 0;

    inline std::unique_ptr<ReadOnlyArchive> findReadOnlyFor(const ModImplementation &mod) override {
        return findFor(mod);
    };
};
