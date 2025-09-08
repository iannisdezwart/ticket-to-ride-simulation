#include "strat/manual/impl/claim_route_decision_generator.hpp"

ClaimRouteDecisionGenerator::ClaimRouteDecisionGenerator(AsyncReader &reader,
                                                         const Game &gameState)
    : reader(reader), gameState(gameState) {}

ClaimRouteDecisionGenerator::Ret ClaimRouteDecisionGenerator::poll() {
  switch (state) {
  case State::Started:
    return started();
  case State::ReadingRouteInput:
    return readingRouteInput();
  case State::WaitingForPayment:
    return waitingForPayment();
  case State::WaitingForTunnelBackupPayment:
    return waitingForTunnelBackupPayment();
  }
}

ClaimRouteDecisionGenerator::Ret ClaimRouteDecisionGenerator::started() {
  std::cout << "The following routes are still unclaimed:" << std::endl;
  for (auto i = 0uz; i < gameState.map.routes.claim.size(); i++) {
    if (gameState.map.routes.claim[i] != -1) {
      continue;
    }
    auto city1 = standardCities[gameState.map.routes.city1[i]];
    auto city2 = standardCities[gameState.map.routes.city2[i]];
    auto colour = gameState.map.routes.colour[i];
    auto length = gameState.map.routes.length[i];
    auto isTunnel = gameState.map.routes.isTunnel[i];
    auto numFerries = gameState.map.routes.numFerries[i];
    std::cout << "[" << i << "] " << city1 << " -> " << city2 << " "
              << int(length) << " " << routeColourToStr(colour);
    if (isTunnel) {
      std::cout << " (isTunnel)";
    }
    if (numFerries != 0) {
      std::cout << " (" << int(numFerries) << " ferries)";
    }
    std::cout << std::endl;
  }

  std::cout << "Choose the index of the route you want to claim:" << std::endl;

  state = State::ReadingRouteInput;
  return readingRouteInput();
}

ClaimRouteDecisionGenerator::Ret
ClaimRouteDecisionGenerator::readingRouteInput() {
  auto line = reader.readline();
  if (!line.has_value()) {
    return std::nullopt;
  }

  try {
    routeIdx = std::stoul(line.value());
    if (routeIdx >= NumRoutes) {
      std::cout << "Specified index out of range. Try choosing again:"
                << std::endl;
      return started();
    }
  } catch (const std::out_of_range &ex) {
    std::cout << "Parse error. Try choosing again:" << std::endl;
    return started();
  }

  paymentGenerator = std::make_unique<PaymentAsyncGenerator>(
      reader, "Choose how to pay for route:");
  state = State::WaitingForPayment;
  return waitingForPayment();
}

ClaimRouteDecisionGenerator::Ret
ClaimRouteDecisionGenerator::waitingForPayment() {
  auto res = paymentGenerator->poll();
  if (!res.has_value()) {
    return std::nullopt;
  }

  payment = res.value();

  if (gameState.map.routes.isTunnel[routeIdx]) {
    paymentGenerator = std::make_unique<PaymentAsyncGenerator>(
        reader, "Choose back-up pre-payment for tunnel:");
    state = State::WaitingForTunnelBackupPayment;
    return waitingForTunnelBackupPayment();
  }

  return finish();
}

ClaimRouteDecisionGenerator::Ret
ClaimRouteDecisionGenerator::waitingForTunnelBackupPayment() {
  auto res = paymentGenerator->poll();
  if (!res.has_value()) {
    return std::nullopt;
  }

  tunnelBackupPayment = res.value();
  return finish();
}

ClaimRouteDecisionGenerator::Ret ClaimRouteDecisionGenerator::finish() {
  return ClaimRouteDecision{
      .routeIdx = routeIdx,
      .payment = payment,
      .tunnelBackupPayment = tunnelBackupPayment,
  };
}