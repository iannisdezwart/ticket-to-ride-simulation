#pragma once

#include "core/destination_ticket_card.hpp"
#include "core/game.hpp"
#include "turn/turn_decision.hpp"
#include "util/async_generator.hpp"
#include <optional>
#include <vector>

struct RoutesStrategy {
  virtual ~RoutesStrategy() noexcept = default;
  virtual std::optional<TurnDecision> operator()(const Game &gameState) = 0;
};
