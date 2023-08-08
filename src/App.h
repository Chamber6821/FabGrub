//
// Created by Ivan on 08.08.2023.
//

#pragma once

#include <utility>

#include "destination/Destination.h"
#include "profile/Profiles.h"
#include "repository/Repository.h"

class App {
    ptr<Profiles> profiles;
    ptr<Repository> repository;
    ptr<Destination> destination;

  public:
    App(ptr<Profiles> profiles, ptr<Repository> repository,
        ptr<Destination> destination)
        : profiles(std::move(profiles)), repository(std::move(repository)),
          destination(std::move(destination)) {}

    void load(std::string profile) {}
};
