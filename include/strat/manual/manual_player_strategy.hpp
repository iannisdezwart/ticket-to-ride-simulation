#pragma once

#include "strat/player_strategy.hpp"
#include "util/async_reader.hpp"

class ManualPlayerStrategy : public PlayerStrategy {
public:
  AsyncGeneratorPtr<uint8_t> chooseInitialDestinationTickets(
      const Map &map,
      const std::array<DestinationTicketCard, 4> &cards) override;

  AsyncGeneratorPtr<TurnDecision> takeTurn(const Game &gameState) override;

private:
  AsyncReader reader;
};