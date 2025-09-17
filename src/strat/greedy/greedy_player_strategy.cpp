#include "strat/greedy/greedy_player_strategy.hpp"
#include "algo/routes_strategy/routes_strategy.hpp"
#include "core/game.hpp"
#include "strat/greedy/impl/draw_new_destination_ticket_cards.hpp"
#include <iostream>
#include <sstream>

GreedyPlayerStrategy::GreedyPlayerStrategy(std::unique_ptr<RoutesStrategy> routesStrategy)
    : routesStrategy(std::move(routesStrategy)) {}

AsyncGeneratorPtr<uint8_t> GreedyPlayerStrategy::chooseInitialDestinationTickets(
    const Map &, const std::array<DestinationTicketCard, 4> &) {
  return plainValue(uint8_t(0b1111));
}

AsyncGeneratorPtr<TurnDecision>
GreedyPlayerStrategy::takeTurn(const Game &gameState) {
  auto routesStrategyRes = (*routesStrategy)(gameState);
  if (routesStrategyRes.has_value()) {
    return plainValue(std::move(routesStrategyRes.value()));
  }

  return plainValue(drawNewDestinationTicketCards());
}
