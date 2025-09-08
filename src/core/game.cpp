#include "core/game.hpp"
#include "strat/player_strategy.hpp"
#include <bit>

Game::Game(std::vector<std::unique_ptr<PlayerStrategy>> playerStrategies)
    : playerStates(playerStrategies.size()),
      playerStrategies(std::move(playerStrategies)), gameInitialiser(*this) {
  std::generate(playerStates.begin(), playerStates.end(),
                [this]() { return PlayerState(trainCarCardDeck); });
  playerIdx = 0;
}

void Game::run() {
  if (!gameInitialiser.done && !gameInitialiser.poll().has_value()) {
    return;
  }
  if (pendingDestinationTicketsChoiceDecision &&
      !awaitPendingDestinationTicketsChoiceDecision()) {
    return;
  }
  if (pendingDrawSecondTrainCarCardDecision &&
      !awaitPendingDrawSecondTrainCarCardDecision()) {
    return;
  }
  if (!pendingTurnDecision) {
    pendingTurnDecision = playerStrategies[playerIdx]->takeTurn(*this);
  }
  auto res = pendingTurnDecision->poll();
  if (!res.has_value()) {
    return;
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

void Game::passTurn() { playerIdx = (playerIdx + 1) % playerStates.size(); }

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

bool matches(TrainCarCard t, RouteColour r) {
  if (r == RouteColour::Grey) {
    return true;
  }
  // clang-format off
  switch (t) {
  case TrainCarCard::Pink:        return r == RouteColour::Pink;
  case TrainCarCard::White:       return r == RouteColour::White;
  case TrainCarCard::Blue:        return r == RouteColour::Blue;
  case TrainCarCard::Yellow:      return r == RouteColour::Yellow;
  case TrainCarCard::Orange:      return r == RouteColour::Orange;
  case TrainCarCard::Black:       return r == RouteColour::Black;
  case TrainCarCard::Red:         return r == RouteColour::Red;
  case TrainCarCard::Green:       return r == RouteColour::Green;
  case TrainCarCard::Locomotive:  return true;
  default: throw std::out_of_range("Invalid TrainCarCard enumeration value.");
  }
  // clang-format on
}

void Game::processClaimRouteDecision(const ClaimRouteDecision &d) {
  auto checker = TrainCarCardHand();
  for (const auto &c : d.payment) {
    checker.add(c);
  }
  for (const auto &c : d.tunnelBackupPayment) {
    checker.add(c);
  }
  if (!checker.subset(playerStates[playerIdx].trainCarCards)) {
    std::cerr << "Player's hand: "
              << playerStates[playerIdx].trainCarCards.report();
    std::cerr << "Payment cards: " << checker.report();
    throw std::runtime_error("Payment is not a subset of hand.");
  }
  if (map.routes.claim[d.routeIdx] != -1) {
    throw std::runtime_error("Tried to claim already claimed route.");
  }
  auto colourUsed = TrainCarCard::Locomotive;
  auto checkColourUsed = [&colourUsed](TrainCarCard c) {
    if (c == TrainCarCard::Locomotive) {
      return;
    }
    if (colourUsed != TrainCarCard::Locomotive && colourUsed != c) {
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
  if (!matches(colourUsed, routeColour)) {
    std::cerr << "colourUsed: " << trainCarCardToStr(colourUsed) << std::endl;
    std::cerr << "routeColour: " << routeColourToStr(routeColour) << std::endl;
    throw std::runtime_error("Wrong colour used in route payment.");
  }
  auto length = map.routes.length[d.routeIdx];
  if (uint8_t(d.payment.size()) != length) {
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
    std::cout << "Tunnel extra cost: " << tunnelExtraCost << std::endl;
    if (int(d.tunnelBackupPayment.size()) < tunnelExtraCost) {
      // Failed to pay extra cost. Route not claimed.
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
    throw std::runtime_error("Another train station is already placed here.");
  }
  auto reqPayment = 4 - playerStates[playerIdx].trainStations;
  if (int(d.payment.size()) != reqPayment) {
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
  passTurn();
}

void Game::processDrawTwoFaceDownTrainCarCardsDecision(
    const DrawTwoFaceDownTrainCarCardsDecision &) {
  playerStates[playerIdx].trainCarCards.add(
      trainCarCardDeck.drawFromFaceDownPile());
  playerStates[playerIdx].trainCarCards.add(
      trainCarCardDeck.drawFromFaceDownPile());
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
    passTurn();
    return;
  }
  throw std::runtime_error("There is no face-up locomotive card.");
}

void Game::processDrawOneFaceUpTrainCarCardDecision(
    const DrawOneFaceUpTrainCarCardDecision &d) {
  playerStates[playerIdx].trainCarCards.add(
      trainCarCardDeck.takeFromFaceUpPile(d.idx));
  handleThreeLocomotivesInFaceUpPile();
  d.secondAction();
}

bool Game::awaitPendingDestinationTicketsChoiceDecision() {
  auto res = pendingDestinationTicketsChoiceDecision->poll();
  if (!res.has_value()) {
    return false;
  }
  pendingDestinationTicketsChoiceDecision = nullptr;
  auto mask = res.value();
  if (std::popcount(mask) < 1) {
    std::cerr << "mask: " << int(mask) << std::endl;
    throw std::runtime_error("Must keep at least one destination ticket.");
  }
  for (auto i = 0; i < 3; i++) {
    if (mask & (1 << i)) {
      auto card = destinationTicketsChoiceDecisionCards[i];
      playerStates[playerIdx].destinationTicketCards.push_back(card);
    }
  }
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
  playerStates[playerIdx].trainCarCards.add(
      trainCarCardDeck.takeFromFaceUpPile(d.idx));
  handleThreeLocomotivesInFaceUpPile();
}

void Game::processDrawSecondFaceDownTrainCarCardDecision(
    const DrawOneFaceUpTrainCarCardDecision::
        DrawSecondFaceDownTrainCarCardDecision &) {
  playerStates[playerIdx].trainCarCards.add(
      trainCarCardDeck.drawFromFaceDownPile());
}