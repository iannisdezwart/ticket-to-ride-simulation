#pragma once

#include "algo/routes_strategy/routes_strategy.hpp"
#include "strat/player_strategy.hpp"
#include "util/async_reader.hpp"

class GreedyPlayerStrategy : public PlayerStrategy {
private:
  std::unique_ptr<RoutesStrategy> routesStrategy;

public:
  GreedyPlayerStrategy() = delete;
  GreedyPlayerStrategy(std::unique_ptr<RoutesStrategy> routesStrategy);

  ~GreedyPlayerStrategy() noexcept = default;

  AsyncGeneratorPtr<uint8_t> chooseInitialDestinationTickets(
      const Map &map,
      const std::array<DestinationTicketCard, 4> &cards) override;

  AsyncGeneratorPtr<TurnDecision> takeTurn(const Game &gameState) override;
};