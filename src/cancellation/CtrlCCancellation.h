#pragma once

#include "Cancellation.h"
#include "ctrl-c/ctrl-c.h"
#include <atomic>
#include <stdexcept>

class CtrlCCancellation : public Cancellation {
    std::atomic<bool> _cancelled = false;
    unsigned int id;

  public:
    CtrlCCancellation() {
        id = CtrlCLibrary::SetCtrlCHandler([&](auto) {
            _cancelled = true;
            return true;
        });
    }

    ~CtrlCCancellation() { CtrlCLibrary::ResetCtrlCHandler(id); }

    auto canceled() const -> bool override { return _cancelled; }

    void throwIfCanceled() const override {
        if (canceled()) throw std::runtime_error("Program was cancelled");
    }
};
