#pragma once

#include "algo/routes_strategy/routes_strategy.hpp"
#include "strat/player_strategy.hpp"
#include "util/async_reader.hpp"

class LongRoutesPlayerStrategy : public PlayerStrategy {
private:
  std::unique_ptr<RoutesStrategy> routesStrategy;

public:
  LongRoutesPlayerStrategy() = delete;
  LongRoutesPlayerStrategy(std::unique_ptr<RoutesStrategy> routesStrategy);

  ~LongRoutesPlayerStrategy() noexcept = default;

  AsyncGeneratorPtr<uint8_t> chooseInitialDestinationTickets(
      const Map &Map,
      const std::array<DestinationTicketCard, 4> &cards) override;

  AsyncGeneratorPtr<TurnDecision> takeTurn(const Game &gameState) override;
};