//
// Created by Ivan on 12.08.2023.
//

#pragma once

#include "Package.h"
#include "choice/Choice.h"

class ChosenPackage : public Package {
    ptr<Choice<ptr<Package>>> choice;

  public:
    explicit ChosenPackage(ptr<Choice<ptr<Package>>> choice)
        : choice(std::move(choice)) {}

    [[nodiscard]] auto name() const -> std::string override {
        return choice->element()->name();
    }

    [[nodiscard]] auto version() const -> ptr<Version> override {
        return choice->element()->version();
    }

    [[nodiscard]] auto requirements() const -> ptr<Requirements> override {
        return choice->element()->requirements();
    }
};
