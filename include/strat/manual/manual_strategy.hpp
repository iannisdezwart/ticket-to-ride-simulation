#pragma once

#include "strat/player_strategy.hpp"
#include "util/async_reader.hpp"

class ManualStrategy : public PlayerStrategy {
public:
  AsyncGeneratorPtr<uint8_t> chooseInitialDestinationTickets(
      std::array<DestinationTicketCard, 4> cards) override;

  AsyncGeneratorPtr<TurnDecision> takeTurn(const Game &gameState) override;

private:
  AsyncReader reader;
};