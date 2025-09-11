#include "strat/manual/impl/take_turn_async_generator.hpp"
#include "strat/manual/impl/build_train_station_decision_generator.hpp"
#include "strat/manual/impl/choose_destination_tickets_async_generator.hpp"
#include "strat/manual/impl/claim_route_decision_generator.hpp"
#include "strat/manual/impl/draw_train_car_cards_decision_generator.hpp"
#include <string>

TakeTurnAsyncGenerator::TakeTurnAsyncGenerator(AsyncReader &reader,
                                               const Game &gameState)
    : reader(reader), gameState(gameState) {}

TakeTurnAsyncGenerator::Ret TakeTurnAsyncGenerator::poll() {
  // clang-format off
  switch (state) {
  case State::Started:          return started();
  case State::ReadingInput:     return readingInput();
  case State::WaitingForChild:  return waitingForChild();
  }
  // clang-format on
}

TakeTurnAsyncGenerator::Ret TakeTurnAsyncGenerator::started() {
  std::cout << "Choose what to do for your turn:" << std::endl;
  std::cout << "[0] Draw train car cards" << std::endl;
  std::cout << "[1] Claim a route" << std::endl;
  std::cout << "[2] Draw destination tickets" << std::endl;
  std::cout << "[3] Build a train station" << std::endl;
  std::cout << "Choose the index of the decision you want to make:"
            << std::endl;

  state = State::ReadingInput;
  return readingInput();
}

TakeTurnAsyncGenerator::Ret TakeTurnAsyncGenerator::readingInput() {
  auto line = reader.readline();
  if (!line.has_value()) {
    return std::nullopt;
  }

  try {
    auto idx = std::stoul(line.value());
    if (idx >= 4) {
      std::cout << "Specified index out of range. Try choosing again:"
                << std::endl;
      return readingInput();
    }
    switch (idx) {
    case 0:
      return drawTrainCarCards();
    case 1:
      return claimRoute();
    case 2:
      return DrawDestinationTicketsDecision{
          .chooseToKeep =
              [this](std::array<DestinationTicketCard, 3> cards) {
                return std::make_unique<
                    ChooseDestinationTicketsAsyncGenerator<3>>(
                    reader, cards,
                    "You've drawn the following new destination tickets:");
              },
      };
    case 3:
      return buildTrainStation();
    default:
      throw std::logic_error("Unreachable");
    }
  } catch (const std::out_of_range &ex) {
    std::cout << "Parse error. Try choosing again:" << std::endl;
    return readingInput();
  }
}

TakeTurnAsyncGenerator::Ret TakeTurnAsyncGenerator::waitingForChild() {
  return child->poll();
}

TakeTurnAsyncGenerator::Ret TakeTurnAsyncGenerator::drawTrainCarCards() {
  child = transform<TurnDecision>(
      std::make_unique<DrawTrainCarCardsDecisionGenerator>(reader, gameState));
  state = State::WaitingForChild;
  return waitingForChild();
}

TakeTurnAsyncGenerator::Ret TakeTurnAsyncGenerator::claimRoute() {
  child = transform<TurnDecision>(
      std::make_unique<ClaimRouteDecisionGenerator>(reader, gameState));
  state = State::WaitingForChild;
  return waitingForChild();
}

TakeTurnAsyncGenerator::Ret TakeTurnAsyncGenerator::buildTrainStation() {
  child = transform<TurnDecision>(
      std::make_unique<BuildTrainStationDecisionGenerator>(reader, gameState));
  state = State::WaitingForChild;
  return waitingForChild();
}