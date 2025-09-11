#include "strat/greedy/greedy_strategy.hpp"
#include "core/game.hpp"
#include "eval/completed_destination_tickets.hpp"
#include "strat/greedy/impl/draw_new_destination_ticket_cards.hpp"
#include "strat/greedy/impl/focus_on_uncompleted_route.hpp"
#include <iostream>
#include <sstream>

AsyncGeneratorPtr<uint8_t> GreedyStrategy::chooseInitialDestinationTickets(
    std::array<DestinationTicketCard, 4>) {
  return plainValue(uint8_t(0b1111));
}

AsyncGeneratorPtr<TurnDecision>
GreedyStrategy::takeTurn(const Game &gameState) {
  auto rep = evalCompletedDestinationTickets(gameState);
  const auto &destinationTicketCards =
      gameState.playerStates[gameState.playerIdx].destinationTicketCards;
  for (auto i = 0uz; i < destinationTicketCards.size(); i++) {
    if (rep.completed[i]) {
      continue;
    }
    return transform<TurnDecision>(plainValue(
        focusOnUncompletedRoute(gameState, destinationTicketCards[i])));
  }
  return transform<TurnDecision>(plainValue(drawNewDestinationTicketCards()));
}
