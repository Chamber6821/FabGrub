#pragma once

#include "Cancellation.h"

class NonCancellable : public Cancellation {
  public:
    [[nodiscard]] auto canceled() const -> bool override { return false; }

    void throwIfCanceled() const override {}
};
