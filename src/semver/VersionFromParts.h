//
// Created by Ivan on 06.08.2023.
//

#pragma once

#include <utility>

#include "Reactions/misc/ScalarBox.h"
#include "Reactions/misc/owning/in.h"
#include "Reactions/misc/owning/make.h"
#include "Version.h"
#include "VersionPart.h"

class VersionFromParts : public Version {
    // VersionPart uses API from Reaction,
    // but Reactions don't use const qualifier
    mutable in<VersionPart> _major;
    mutable in<VersionPart> _minor;
    mutable in<VersionPart> _patch;

  public:
    VersionFromParts(
        in<VersionPart> major, in<VersionPart> minor, in<VersionPart> patch
    )
        : _major(std::move(major)), _minor(std::move(minor)),
          _patch(std::move(patch)) {}

    VersionFromParts(int major, int minor, int patch)
        : VersionFromParts(
              make<ScalarBox<int>>(major), make<ScalarBox<int>>(minor),
              make<ScalarBox<int>>(patch)
          ) {}

    VersionFromParts() : VersionFromParts(0, 0, 0) {}

    auto major() const -> int override { return _major->value(); };

    auto minor() const -> int override { return _minor->value(); };

    auto patch() const -> int override { return _patch->value(); };
};
