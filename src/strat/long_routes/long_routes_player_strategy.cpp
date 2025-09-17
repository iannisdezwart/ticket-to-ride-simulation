#include "strat/long_routes/long_routes_player_strategy.hpp"
#include "algo/routes_strategy/routes_strategy.hpp"
#include "core/game.hpp"
#include "strat/long_routes/impl/build_longest_route_on_board.hpp"
#include "strat/long_routes/impl/choose_easiest_initial_destination_tickets.hpp"
#include <iostream>
#include <sstream>

LongRoutesPlayerStrategy::LongRoutesPlayerStrategy(
    std::unique_ptr<RoutesStrategy> routesStrategy)
    : routesStrategy(std::move(routesStrategy)) {}

AsyncGeneratorPtr<uint8_t>
LongRoutesPlayerStrategy::chooseInitialDestinationTickets(
    const Map &map, const std::array<DestinationTicketCard, 4> &cards) {
  return plainValue(chooseEasiestInitialDestinationTickets(map, cards));
}

AsyncGeneratorPtr<TurnDecision>
LongRoutesPlayerStrategy::takeTurn(const Game &gameState) {
  auto routesStrategyRes = (*routesStrategy)(gameState);
  if (routesStrategyRes.has_value()) {
    return plainValue(std::move(routesStrategyRes.value()));
  }

  return plainValue(buildLongestRouteOnBoard(gameState));
}
