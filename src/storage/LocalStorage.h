//
// Created by Ivan on 26.01.2023.
//

#pragma once

#include "Storage.h"

class LocalStorage : public Storage {
    class LocalArchive : public Archive {
        std::filesystem::path file;

      public:
        /* Create hard link from original file to storage.folder() */
        bool andCloneTo(Storage &storage) override;
        bool andRemove() override;
    };

  public:
    std::unique_ptr<Archive> findFor(const ModImplementation &mod) override;
    std::filesystem::path folder() override;
    void clear() override;
};
