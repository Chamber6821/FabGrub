//
// Created by Ivan on 26.01.2023.
//

#pragma once

#include "ReadOnlyStorage.h"
#include <string>

class PortalStorage : public ReadOnlyStorage {
    class RemoteArchive : public ReadOnlyArchive {
        std::string url;
        std::string filename;

      public:
        bool andCloneTo(Storage &storage) override;
    };

  public:
    std::unique_ptr<ReadOnlyArchive> findReadOnlyFor(const ModImplementation &mod) override;
};
