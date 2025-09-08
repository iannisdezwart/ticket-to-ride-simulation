#include "strat/manual/impl/draw_train_car_cards_decision_generator.hpp"
#include "strat/manual/impl/draw_second_train_car_card_decision_generator.hpp"
#include <iostream>

DrawTrainCarCardsDecisionGenerator::DrawTrainCarCardsDecisionGenerator(
    AsyncReader &reader, const Game &gameState)
    : reader(reader), gameState(gameState) {}

DrawTrainCarCardsDecisionGenerator::Ret
DrawTrainCarCardsDecisionGenerator::poll() {
  switch (state) {
  case State::Started:
    return started();
  case State::ReadingDecisionInput:
    return readingDecisionInput();
  case State::ReadingDrawOneFaceUpDecisionInput:
    return readingDrawOneFaceUpDecisionInput();
  }
}

DrawTrainCarCardsDecisionGenerator::Ret
DrawTrainCarCardsDecisionGenerator::started() {
  std::cout << "Choose how to draw train car cards:" << std::endl;
  std::cout << "[0] Draw two face-down train car cards" << std::endl;
  std::cout << "[1] Draw one face-up locomotive card" << std::endl;
  std::cout << "[2] Draw a face-up non-locomotive card" << std::endl;
  std::cout << "Choose the index of the decision you want to make:"
            << std::endl;

  state = State::ReadingDecisionInput;
  return readingDecisionInput();
}

DrawTrainCarCardsDecisionGenerator::Ret
DrawTrainCarCardsDecisionGenerator::readingDecisionInput() {
  auto line = reader.readline();
  if (!line.has_value()) {
    return std::nullopt;
  }

  try {
    auto idx = std::stoul(line.value());
    if (idx >= 3) {
      std::cout << "Specified index out of range. Try choosing again:"
                << std::endl;
      return started();
    }
    switch (idx) {
    case 0:
      return DrawTwoFaceDownTrainCarCardsDecision();
    case 1:
      return DrawOneFaceUpLocomotiveCardDecision();
    case 2:
      return drawFaceUpTrainCarCardDecision();
    default:
      throw std::logic_error("Unreachable");
    }
  } catch (const std::out_of_range &ex) {
    std::cout << "Parse error. Try choosing again:" << std::endl;
    return started();
  }
}

DrawTrainCarCardsDecisionGenerator::Ret
DrawTrainCarCardsDecisionGenerator::drawFaceUpTrainCarCardDecision() {
  auto faceUpCards = gameState.trainCarCardDeck.getFaceUpCards();
  std::cout << "Face-up cards:" << std::endl;
  for (auto i = 0uz; i < faceUpCards.size(); i++) {
    std::cout << "[" << i << "] " << trainCarCardToStr(faceUpCards[i])
              << std::endl;
  }
  std::cout << "Select the index of the face-up train car card to pick:";

  state = State::ReadingDrawOneFaceUpDecisionInput;
  return readingDrawOneFaceUpDecisionInput();
}

DrawTrainCarCardsDecisionGenerator::Ret
DrawTrainCarCardsDecisionGenerator::readingDrawOneFaceUpDecisionInput() {
  auto line = reader.readline();
  if (!line.has_value()) {
    return std::nullopt;
  }

  try {
    uint8_t idx = std::stoul(line.value());
    if (idx >= 5) {
      std::cout << "Specified index out of range. Try choosing again:"
                << std::endl;
      return drawFaceUpTrainCarCardDecision();
    }
    return DrawOneFaceUpTrainCarCardDecision{
        .idx = idx,
        .secondAction =
            [this]() {
              return std::make_unique<DrawSecondTrainCarCardDecisionGenerator>(
                  reader, gameState);
            },
    };
  } catch (const std::out_of_range &ex) {
    std::cout << "Parse error. Try choosing again:" << std::endl;
    return drawFaceUpTrainCarCardDecision();
  }
}