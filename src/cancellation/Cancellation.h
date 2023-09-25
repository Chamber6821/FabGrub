#pragma once

#include "Reactions/misc/Interface.h"

class Cancellation : public Interface {
  public:
    [[nodiscard]] virtual auto canceled() const -> bool = 0;
    virtual void throwIfCanceled() const = 0;
};
