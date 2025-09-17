#include "strat/long_routes/impl/build_longest_route_on_board.hpp"

TurnDecision buildLongestRouteOnBoard(const Game &game) {
  auto longestRouteIdx = int8_t(-1);
  auto longestRouteLength = uint8_t(0);
  auto availableTrainCars = game.playerStates[game.playerIdx].trainCars;

  for (auto i = 0; i < NumRoutes; i++) {
    if (game.map.routes.claim[i] != -1) {
      continue;
    }
    auto length = game.map.routes.length[i];
    if (length > availableTrainCars) {
      continue;
    }
    if (length > longestRouteLength) {
      longestRouteIdx = static_cast<uint8_t>(i);
      longestRouteLength = length;
    }
  }
  const auto &hand = game.playerStates[game.playerIdx].trainCarCards;
  if (longestRouteIdx == -1) {
    return DrawTwoFaceDownTrainCarCardsDecision();
  }
  auto paymentOptions = routePaymentOptions(hand, game.map, longestRouteIdx);
  if (paymentOptions.empty()) {
    return DrawTwoFaceDownTrainCarCardsDecision();
  }
  return ClaimRouteDecision{
      .routeIdx = static_cast<uint8_t>(longestRouteIdx),
      .payment = paymentOptions.front().payment,
      .tunnelBackupPayment = paymentOptions.front().tunnelBackupPayment,
  };
}