#include "core/game.hpp"
#include "algo/train_car_card_matches_route_colour.hpp"
#include "eval/completed_routes.hpp"
#include "eval/european_express_bonus.hpp"
#include "strat/player_strategy.hpp"
#include <bit>

Game::Game(std::vector<std::unique_ptr<PlayerStrategy>> playerStrategies)
    : playerStates(playerStrategies.size()),
      playerStrategies(std::move(playerStrategies)), gameInitialiser(*this) {
  std::generate(playerStates.begin(), playerStates.end(),
                [this]() { return PlayerState(trainCarCardDeck); });
  playerIdx = 0;
}

bool Game::run() {
  if (numTurnsUntilEnd == 0) {
    return false;
  }
  if (!gameInitialiser.done && !gameInitialiser.poll().has_value()) {
    return true;
  }
  if (pendingDestinationTicketsChoiceDecision &&
      !awaitPendingDestinationTicketsChoiceDecision()) {
    return true;
  }
  if (pendingDrawSecondTrainCarCardDecision &&
      !awaitPendingDrawSecondTrainCarCardDecision()) {
    return true;
  }
  if (!pendingTurnDecision) {
    pendingTurnDecision = playerStrategies[playerIdx]->takeTurn(*this);
  }
  auto res = pendingTurnDecision->poll();
  if (!res.has_value()) {
    return true;
  }
  std::visit(
      [this](auto &&v) {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, DrawTrainCarCardsDecision>) {
          processDrawTrainCarCardsDecision(v);
        } else if constexpr (std::is_same_v<T, ClaimRouteDecision>) {
          processClaimRouteDecision(v);
        } else if constexpr (std::is_same_v<T,
                                            DrawDestinationTicketsDecision>) {
          processDrawDestinationTicketsDecision(v);
        } else if constexpr (std::is_same_v<T, BuildTrainStationDecision>) {
          processBuildTrainStationDecision(v);
        } else {
          throw std::logic_error("Unhandled TurnDecision case.");
        }
      },
      res.value());
  pendingTurnDecision = nullptr;
  return true;
}

std::vector<PlayerReport> Game::makePlayerReports() {
  auto numPlayers = uint8_t(playerStrategies.size());
  auto europeanExpressBonusPoints =
      evalEuropeanExpressBonus(numPlayers, map.routes);
  auto completedRoutesPoints = evalCompletedRoutes(numPlayers, map.routes);
  auto reps = std::vector<PlayerReport>(numPlayers);
  for (auto playerIdx = 0; playerIdx < numPlayers; playerIdx++) {
    auto &rep = reps[playerIdx];
    rep.playerIdx = playerIdx;
    rep.completedDestinationTickets = evalCompletedDestinationTickets(
        playerIdx, map, playerStates[playerIdx].destinationTicketCards);
    rep.unplayedTrainStationPoints = playerStates[playerIdx].trainStations * 4;
    rep.europeanExpressBonusPoints = europeanExpressBonusPoints[playerIdx];
    rep.completedRoutesPoints = completedRoutesPoints[playerIdx];
    rep.totalPoints =
        rep.completedRoutesPoints + rep.completedDestinationTickets.points +
        rep.unplayedTrainStationPoints + rep.europeanExpressBonusPoints;
  }
  std::sort(reps.begin(), reps.end(), [](const auto &a, const auto &b) {
    return a.totalPoints > b.totalPoints;
  });
  return reps;
}

void Game::handleThreeLocomotivesInFaceUpPile() {
  auto numLocomotives = 0;
  const auto faceUpCards = trainCarCardDeck.getFaceUpCards();
  for (const auto &c : faceUpCards) {
    if (c == TrainCarCard::Locomotive) {
      numLocomotives++;
    }
  }

  if (numLocomotives < 3) {
    return;
  }

  std::cout << "Three locomotives in face-up train car cards, shuffling..."
            << std::endl;
  for (auto i = 0; i < 5; i++) {
    trainCarCardDeck.discard(trainCarCardDeck.cards[i]);
  }
  for (auto i = 0; i < 5; i++) {
    trainCarCardDeck.cards[i] = trainCarCardDeck.drawFromFaceDownPile();
  }

  handleThreeLocomotivesInFaceUpPile();
}

void Game::passTurn() {
  playerIdx = (playerIdx + 1) % playerStates.size();
  if (numTurnsUntilEnd != -1) {
    numTurnsUntilEnd--;
  }
}

void Game::checkIfGameIsFinishing() {
  if (numTurnsUntilEnd != -1) {
    return;
  }
  if (playerStates[playerIdx].trainCars <= 2) {
    numTurnsUntilEnd = playerStates.size() + 1;
    std::cout << "Game is finishing..." << std::endl;
    std::cout << int(numTurnsUntilEnd - 1) << " more turns until end."
              << std::endl;
  }
}

void Game::processDrawTrainCarCardsDecision(
    const DrawTrainCarCardsDecision &d) {
  std::visit(
      [this](auto &&v) {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, DrawTwoFaceDownTrainCarCardsDecision>) {
          processDrawTwoFaceDownTrainCarCardsDecision(v);
        } else if constexpr (std::is_same_v<
                                 T, DrawOneFaceUpLocomotiveCardDecision>) {
          processDrawOneFaceUpLocomotiveCardDecision(v);
        } else if constexpr (std::is_same_v<
                                 T, DrawOneFaceUpTrainCarCardDecision>) {
          processDrawOneFaceUpTrainCarCardDecision(v);
        } else {
          throw std::logic_error("Unhandled TurnDecision case.");
        }
      },
      d);
}

void Game::processClaimRouteDecision(const ClaimRouteDecision &d) {
  auto length = map.routes.length[d.routeIdx];
  auto availableTrainCars = playerStates[playerIdx].trainCars;
  if (availableTrainCars < length) {
    std::cerr << "Route: " << standardCities[map.routes.city1[d.routeIdx]]
              << " -> " << standardCities[map.routes.city2[d.routeIdx]]
              << std::endl;
    std::cerr << "Available train cars: " << int(availableTrainCars)
              << std::endl;
    std::cerr << "Route length: " << int(length) << std::endl;
    throw std::runtime_error("Route uses more train cars than available.");
  }
  auto checker = TrainCarCardHand();
  for (const auto &c : d.payment) {
    checker.add(c);
  }
  for (const auto &c : d.tunnelBackupPayment) {
    checker.add(c);
  }
  if (!checker.subset(playerStates[playerIdx].trainCarCards)) {
    std::cerr << "Route: " << standardCities[map.routes.city1[d.routeIdx]]
              << " -> " << standardCities[map.routes.city2[d.routeIdx]]
              << std::endl;
    std::cerr << "Player's hand: "
              << playerStates[playerIdx].trainCarCards.report();
    std::cerr << "Payment cards: " << checker.report();
    throw std::runtime_error("Payment is not a subset of hand.");
  }
  if (map.routes.claim[d.routeIdx] != -1) {
    std::cerr << "Route: " << standardCities[map.routes.city1[d.routeIdx]]
              << " -> " << standardCities[map.routes.city2[d.routeIdx]]
              << std::endl;
    std::cerr << "Claim: player #" << int(map.routes.claim[d.routeIdx])
              << std::endl;
    throw std::runtime_error("Tried to claim already claimed route.");
  }
  auto colourUsed = TrainCarCard::Locomotive;
  auto checkColourUsed = [&colourUsed, this, &d](TrainCarCard c) {
    if (c == TrainCarCard::Locomotive) {
      return;
    }
    if (colourUsed != TrainCarCard::Locomotive && colourUsed != c) {
      std::cerr << "Route: " << standardCities[map.routes.city1[d.routeIdx]]
                << " -> " << standardCities[map.routes.city2[d.routeIdx]]
                << std::endl;
      std::cerr << "Colour #1: " << trainCarCardToStr(colourUsed) << std::endl;
      std::cerr << "Colour #2: " << trainCarCardToStr(c) << std::endl;
      throw std::runtime_error("More than one colour used in route payment.");
    }
    colourUsed = c;
  };
  for (const auto &c : d.payment) {
    checkColourUsed(c);
  }
  for (const auto &c : d.tunnelBackupPayment) {
    checkColourUsed(c);
  }
  auto routeColour = map.routes.colour[d.routeIdx];
  if (!trainCarCardMatchesRouteColour(colourUsed, routeColour)) {
    std::cerr << "Route: " << standardCities[map.routes.city1[d.routeIdx]]
              << " -> " << standardCities[map.routes.city2[d.routeIdx]]
              << std::endl;
    std::cerr << "colourUsed: " << trainCarCardToStr(colourUsed) << std::endl;
    std::cerr << "routeColour: " << routeColourToStr(routeColour) << std::endl;
    throw std::runtime_error("Wrong colour used in route payment.");
  }
  if (uint8_t(d.payment.size()) != length) {
    std::cerr << "Route: " << standardCities[map.routes.city1[d.routeIdx]]
              << " -> " << standardCities[map.routes.city2[d.routeIdx]]
              << std::endl;
    std::cerr << "paymentSize: " << d.payment.size() << std::endl;
    std::cerr << "length: " << int(length) << std::endl;
    throw std::runtime_error("Route payment does not match route length.");
  }
  auto paymentForFerries = int(map.routes.numFerries[d.routeIdx]);
  for (const auto &c : d.payment) {
    if (c == TrainCarCard::Locomotive) {
      paymentForFerries--;
    }
  }
  if (paymentForFerries > 0) {
    std::cerr << "Route: " << standardCities[map.routes.city1[d.routeIdx]]
              << " -> " << standardCities[map.routes.city2[d.routeIdx]]
              << std::endl;
    std::cerr << "numFerries: " << int(map.routes.numFerries[d.routeIdx])
              << std::endl;
    throw std::runtime_error("Ferry cost not accounted for in route payment.");
  }
  auto tunnelExtraCost = 0;
  if (map.routes.isTunnel[d.routeIdx]) {
    for (auto i = 0; i < 3; i++) {
      auto card = trainCarCardDeck.drawFromFaceDownPile();
      trainCarCardDeck.discard(card);
      if (card == TrainCarCard::Locomotive || card == colourUsed) {
        tunnelExtraCost++;
      }
    }
    if (int(d.tunnelBackupPayment.size()) < tunnelExtraCost) {
      // Failed to pay extra cost. Route not claimed.
      std::cout << "Player #" << int(playerIdx)
                << " tried to build a tunnel route from "
                << standardCities[map.routes.city1[d.routeIdx]] << " to "
                << standardCities[map.routes.city2[d.routeIdx]]
                << ", but failed to pay the tunnel extra cost." << std::endl;
      passTurn();
      return;
    }
  }
  map.routes.claim[d.routeIdx] = playerIdx;
  playerStates[playerIdx].trainCars -= length;
  for (const auto &c : d.payment) {
    playerStates[playerIdx].trainCarCards.remove(c);
    trainCarCardDeck.discard(c);
  }
  for (auto i = 0; i < tunnelExtraCost; i++) {
    auto c = d.tunnelBackupPayment[i];
    playerStates[playerIdx].trainCarCards.remove(c);
    trainCarCardDeck.discard(c);
  }
  std::cout << "Player #" << int(playerIdx) << " claimed the route from "
            << standardCities[map.routes.city1[d.routeIdx]] << " to "
            << standardCities[map.routes.city2[d.routeIdx]] << "." << std::endl;
  checkIfGameIsFinishing();
  passTurn();
}

void Game::processDrawDestinationTicketsDecision(
    const DrawDestinationTicketsDecision &d) {
  destinationTicketsChoiceDecisionCards = {
      destinationTicketCardDeck.drawRegularCard(),
      destinationTicketCardDeck.drawRegularCard(),
      destinationTicketCardDeck.drawRegularCard(),
  };
  pendingDestinationTicketsChoiceDecision =
      d.chooseToKeep(destinationTicketsChoiceDecisionCards);
}

void Game::processBuildTrainStationDecision(
    const BuildTrainStationDecision &d) {
  if (map.trainStationClaims[d.cityIdx] != -1) {
    std::cerr << "City: " << standardCities[d.cityIdx] << std::endl;
    throw std::runtime_error("Another train station is already placed here.");
  }
  auto reqPayment = 4 - playerStates[playerIdx].trainStations;
  if (int(d.payment.size()) != reqPayment) {
    std::cerr << "City: " << standardCities[d.cityIdx] << std::endl;
    std::cerr << "paymentSize: " << d.payment.size() << std::endl;
    std::cerr << "reqPayment: " << reqPayment << std::endl;
    throw std::runtime_error("Train station payment does not match cost.");
  }
  auto colourUsed = TrainCarCard::Locomotive;
  for (const auto &c : d.payment) {
    if (c == TrainCarCard::Locomotive) {
      continue;
    }
    if (colourUsed != TrainCarCard::Locomotive && colourUsed != c) {
      std::cerr << "City: " << standardCities[d.cityIdx] << std::endl;
      std::cerr << "Colour #1: " << trainCarCardToStr(colourUsed) << std::endl;
      std::cerr << "Colour #2: " << trainCarCardToStr(c) << std::endl;
      throw std::runtime_error(
          "More than one colour used in train station payment.");
    }
    colourUsed = c;
  }
  map.trainStationClaims[d.cityIdx] = playerIdx;
  playerStates[playerIdx].trainStations--;
  for (const auto &c : d.payment) {
    playerStates[playerIdx].trainCarCards.remove(c);
    trainCarCardDeck.discard(c);
  }
  std::cout << "Player #" << int(playerIdx) << " built a train station in "
            << standardCities[d.cityIdx] << "." << std::endl;
  passTurn();
}

void Game::processDrawTwoFaceDownTrainCarCardsDecision(
    const DrawTwoFaceDownTrainCarCardsDecision &) {
  playerStates[playerIdx].trainCarCards.add(
      trainCarCardDeck.drawFromFaceDownPile());
  playerStates[playerIdx].trainCarCards.add(
      trainCarCardDeck.drawFromFaceDownPile());
  std::cout << "Player #" << int(playerIdx)
            << " drew two face-down train car cards." << std::endl;
  passTurn();
}

void Game::processDrawOneFaceUpLocomotiveCardDecision(
    const DrawOneFaceUpLocomotiveCardDecision &) {
  auto faceUpCards = trainCarCardDeck.getFaceUpCards();
  for (auto i = 0uz; i < faceUpCards.size(); i++) {
    if (faceUpCards[i] != TrainCarCard::Locomotive) {
      continue;
    }
    playerStates[playerIdx].trainCarCards.add(
        trainCarCardDeck.takeFromFaceUpPile(i));
    std::cout << "Player #" << int(playerIdx)
              << " drew a face-up locomotive card." << std::endl;
    passTurn();
    return;
  }
  throw std::runtime_error("There is no face-up locomotive card.");
}

void Game::processDrawOneFaceUpTrainCarCardDecision(
    const DrawOneFaceUpTrainCarCardDecision &d) {
  auto card = trainCarCardDeck.takeFromFaceUpPile(d.idx);
  playerStates[playerIdx].trainCarCards.add(card);
  handleThreeLocomotivesInFaceUpPile();
  std::cout << "Player #" << int(playerIdx) << " drew a face-up "
            << trainCarCardToStr(card) << " train car card." << std::endl;
  pendingDrawSecondTrainCarCardDecision = d.secondAction();
}

bool Game::awaitPendingDestinationTicketsChoiceDecision() {
  auto res = pendingDestinationTicketsChoiceDecision->poll();
  if (!res.has_value()) {
    return false;
  }
  pendingDestinationTicketsChoiceDecision = nullptr;
  auto mask = res.value();
  if (std::popcount(mask) < 1) {
    std::cerr << "playerIdx: " << int(playerIdx) << std::endl;
    std::cerr << "mask: " << int(mask) << std::endl;
    throw std::runtime_error("Must keep at least one destination ticket.");
  }
  for (auto i = 0; i < 3; i++) {
    if (mask & (1 << i)) {
      auto card = destinationTicketsChoiceDecisionCards[i];
      playerStates[playerIdx].destinationTicketCards.push_back(card);
    }
  }
  std::cout << "Player #" << int(playerIdx)
            << " drew 3 new destination tickets and decided to keep "
            << std::popcount(mask) << "." << std::endl;
  passTurn();
  return true;
}

bool Game::awaitPendingDrawSecondTrainCarCardDecision() {
  auto res = pendingDrawSecondTrainCarCardDecision->poll();
  if (!res.has_value()) {
    return false;
  }
  pendingDrawSecondTrainCarCardDecision = nullptr;
  std::visit(
      [this](auto &&v) {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<
                          T, DrawOneFaceUpTrainCarCardDecision::
                                 DrawSecondFaceUpTrainCarCardDecision>) {
          processDrawSecondFaceUpTrainCarCardDecision(v);
        } else if constexpr (std::is_same_v<
                                 T,
                                 DrawOneFaceUpTrainCarCardDecision::
                                     DrawSecondFaceDownTrainCarCardDecision>) {
          processDrawSecondFaceDownTrainCarCardDecision(v);
        } else {
          throw std::logic_error("Unhandled TurnDecision case.");
        }
      },
      res.value());
  passTurn();
  return true;
}

void Game::processDrawSecondFaceUpTrainCarCardDecision(
    const DrawOneFaceUpTrainCarCardDecision::
        DrawSecondFaceUpTrainCarCardDecision &d) {
  auto card = trainCarCardDeck.takeFromFaceUpPile(d.idx);
  playerStates[playerIdx].trainCarCards.add(card);
  std::cout << "Player #" << int(playerIdx) << " drew a face-up "
            << trainCarCardToStr(card) << " train car card." << std::endl;
  handleThreeLocomotivesInFaceUpPile();
}

void Game::processDrawSecondFaceDownTrainCarCardDecision(
    const DrawOneFaceUpTrainCarCardDecision::
        DrawSecondFaceDownTrainCarCardDecision &) {
  playerStates[playerIdx].trainCarCards.add(
      trainCarCardDeck.drawFromFaceDownPile());
  std::cout << "Player #" << int(playerIdx)
            << " drew a face-down train card card." << std::endl;
}
