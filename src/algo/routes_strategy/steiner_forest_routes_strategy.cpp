#include "algo/routes_strategy/steiner_forest_routes_strategy.hpp"
#include "algo/path_possible.hpp"
#include "algo/route_payment_options.hpp"
#include "algo/steiner_forest.hpp"
#include "eval/completed_destination_tickets.hpp"
#include <iostream>

bool SteinerForestRoutesStrategy::cacheInvalid(uint8_t cardsSize,
                                               const Game &gameState) {
  if (cardsSize != numDestinationTicketCards) {
    return true;
  }
  for (auto routeIdx : cache) {
    auto claim = gameState.map.routes.claim[routeIdx];
    if (claim != -1 && claim != gameState.playerIdx) {
      return true;
    }
  }

  return false;
}

std::optional<TurnDecision>
SteinerForestRoutesStrategy::operator()(const Game &gameState) {
  auto rep = evalCompletedDestinationTickets(gameState);
  if (std::all_of(rep.completed.begin(), rep.completed.end(),
                  [](auto b) { return b; })) {
    return std::nullopt;
  }

  const auto &cards =
      gameState.playerStates[gameState.playerIdx].destinationTicketCards;
  auto paths = std::vector<std::pair<uint8_t, uint8_t>>();
  paths.reserve(cards.size());
  for (auto i = 0uz; i < cards.size(); i++) {
    const auto &c = cards[i];
    if (!rep.completed[i] &&
        pathPossible(gameState.map, gameState.playerIdx, c.city1, c.city2)) {
      paths.push_back(std::make_pair(c.city1, c.city2));
    }
  }
  if (paths.empty()) {
    return std::nullopt;
  }

  if (cacheInvalid(cards.size(), gameState)) {
    cache = steinerForest(gameState.map, gameState.playerIdx, paths);
    std::cout << "Steiner forest result:" << std::endl;
    for (auto routeIdx : cache) {
      std::cout << " - " << standardCities[gameState.map.routes.city1[routeIdx]]
                << " -> "
                << standardCities[gameState.map.routes.city2[routeIdx]]
                << std::endl;
    }
  }
  numDestinationTicketCards = cards.size();
  std::cout << "Using cached steiner forest result." << std::endl;

  const auto &hand = gameState.playerStates[gameState.playerIdx].trainCarCards;
  for (auto routeIdx : cache) {
    if (gameState.map.routes.claim[routeIdx] != -1) {
      continue;
    }
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
  std::cout << "Can't afford any needed routes. Will draw cards." << std::endl;
  return DrawTwoFaceDownTrainCarCardsDecision{};
}