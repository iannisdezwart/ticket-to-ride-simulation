#include "strat/manual/manual_player_strategy.hpp"
#include "core/game.hpp"
#include "strat/manual/impl/choose_destination_tickets_async_generator.hpp"
#include "strat/manual/impl/take_turn_async_generator.hpp"
#include <iostream>
#include <sstream>

AsyncGeneratorPtr<uint8_t>
ManualPlayerStrategy::chooseInitialDestinationTickets(
    const Map &, const std::array<DestinationTicketCard, 4> &cards) {
  return std::make_unique<ChooseDestinationTicketsAsyncGenerator<4>>(
      reader, std::move(cards), "You've received the following routes:");
}

AsyncGeneratorPtr<TurnDecision>
ManualPlayerStrategy::takeTurn(const Game &gameState) {
  return std::make_unique<TakeTurnAsyncGenerator>(reader, gameState);
}
