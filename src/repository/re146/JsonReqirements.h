//
// Created by Ivan on 16.08.2023.
//

#pragma once

#include "FactorioReqirement.h"
#include "nlohmann/json.hpp"
#include "requirement/MemRequirement.h"
#include "requirement/MemRequirements.h"
#include "utils/range.h"
#include <concepts>
#include <ranges>

namespace re146 {

class JsonRequirements : public MemRequirements {
  public:
    explicit JsonRequirements(const nlohmann::json &reqs)
        : MemRequirements(
              reqs | std::views::transform(&nlohmann::json::get<std::string>) |
              std::views::filter([](const std::string &req) {
                  return not req.starts_with("?") and
                         not req.starts_with("(?)") and
                         not req.starts_with("!");
              }) |
              std::views::transform([](const auto &l) {
                  return make<FactorioRequirement>(l);
              })
          ) {}
};

} // namespace re146