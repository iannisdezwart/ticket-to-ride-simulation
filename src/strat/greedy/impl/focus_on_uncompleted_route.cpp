#include "strat/greedy/impl/focus_on_uncompleted_route.hpp"
#include "algo/route_payment_options.hpp"
#include "algo/shortest_path.hpp"
#include <iostream>

TurnDecision focusOnUncompletedRoute(const Game &gameState,
                                     const DestinationTicketCard &card) {
  std::cout << "Focusing on uncompleted route " << standardCities[card.city1]
            << " -> " << standardCities[card.city2] << "." << std::endl;
  const auto &hand = gameState.playerStates[gameState.playerIdx].trainCarCards;
  auto shPath =
      shortestPath(gameState.map, gameState.playerIdx, card.city1, card.city2);
  std::cout << "Shortest path:" << std::endl;
  for (auto routeIdx : shPath) {
    std::cout << " - " << standardCities[gameState.map.routes.city1[routeIdx]]
              << " -> " << standardCities[gameState.map.routes.city2[routeIdx]]
              << std::endl;
  }
  for (auto routeIdx : shPath) {
    auto paymentOptions = routePaymentOptions(hand, gameState.map, routeIdx);
    if (paymentOptions.empty()) {
      continue;
    }
    std::cout << "Can afford "
              << standardCities[gameState.map.routes.city1[routeIdx]] << " -> "
              << standardCities[gameState.map.routes.city2[routeIdx]]
              << ". Will claim." << std::endl;
    return ClaimRouteDecision{
        .routeIdx = routeIdx,
        .payment = paymentOptions.front().payment,
        .tunnelBackupPayment = paymentOptions.front().tunnelBackupPayment,
    };
  }
  std::cout << "Can't afford any of the routes. Will draw cards." << std::endl;
  return DrawTwoFaceDownTrainCarCardsDecision{};
}