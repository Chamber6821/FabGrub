//
// Created by Ivan on 16.08.2023.
//

#pragma once

#include "Http.h"

class HttpClient : public Http {
  public:
    [[nodiscard]] auto content(const std::string &url) const -> std::string override;
};
