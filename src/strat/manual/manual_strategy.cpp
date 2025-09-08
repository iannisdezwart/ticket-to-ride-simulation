#include "strat/manual/manual_strategy.hpp"
#include "core/game.hpp"
#include "strat/manual/impl/choose_destination_tickets_async_generator.hpp"
#include "strat/manual/impl/take_turn_async_generator.hpp"
#include <iostream>
#include <sstream>

AsyncGeneratorPtr<uint8_t> ManualStrategy::chooseInitialDestinationTickets(
    std::array<DestinationTicketCard, 4> cards) {
  return std::make_unique<ChooseDestinationTicketsAsyncGenerator<4>>(
      reader, std::move(cards), "You've received the following routes:");
}

AsyncGeneratorPtr<TurnDecision>
ManualStrategy::takeTurn(const Game &gameState) {
    return std::make_unique<TakeTurnAsyncGenerator>(reader, gameState);
}
