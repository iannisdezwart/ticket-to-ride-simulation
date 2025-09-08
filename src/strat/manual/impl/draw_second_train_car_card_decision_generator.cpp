#include "strat/manual/impl/draw_second_train_car_card_decision_generator.hpp"

DrawSecondTrainCarCardDecisionGenerator::
    DrawSecondTrainCarCardDecisionGenerator(AsyncReader &reader,
                                            const Game &gameState)
    : reader(reader), gameState(gameState) {}

DrawSecondTrainCarCardDecisionGenerator::Ret
DrawSecondTrainCarCardDecisionGenerator::poll() {
  // clang-format off
  switch (state) {
  case State::Started:       return started();
  case State::ReadingInput:  return readingInput();
  }
  // clang-format on
}

DrawSecondTrainCarCardDecisionGenerator::Ret
DrawSecondTrainCarCardDecisionGenerator::started() {
  auto faceUpCards = gameState.trainCarCardDeck.getFaceUpCards();
  std::cout << "Face-up cards:" << std::endl;
  for (auto i = 0uz; i < faceUpCards.size(); i++) {
    std::cout << "[" << i << "] " << trainCarCardToStr(faceUpCards[i])
              << std::endl;
  }

  std::cout << "If you want to draw a face-down card, hit enter." << std::endl;
  std::cout << "Else, if you want to select another face-up train car "
               "card, enter its index:"
            << std::endl;

  state = State::ReadingInput;
  return readingInput();
}

DrawSecondTrainCarCardDecisionGenerator::Ret
DrawSecondTrainCarCardDecisionGenerator::readingInput() {
  auto line = reader.readline();
  if (!line.has_value()) {
    return std::nullopt;
  }

  if (line.value().size() == 0) {
    return DrawOneFaceUpTrainCarCardDecision::
        DrawSecondFaceDownTrainCarCardDecision();
  }

  try {
    uint8_t idx = std::stoul(line.value());
    if (idx >= 5) {
      std::cout << "Specified index out of range. Try choosing again:"
                << std::endl;
      return started();
    }
    return DrawOneFaceUpTrainCarCardDecision::
        DrawSecondFaceUpTrainCarCardDecision{
            .idx = idx,
        };
  } catch (const std::out_of_range &ex) {
    std::cout << "Parse error. Try choosing again:" << std::endl;
    return started();
  }
}