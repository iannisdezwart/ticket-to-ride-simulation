#pragma once

#include "core/game.hpp"
#include "turn/turn_decision.hpp"
#include "util/async_generator.hpp"
#include "util/async_reader.hpp"

class DrawSecondTrainCarCardDecisionGenerator
    : public AsyncGenerator<DrawOneFaceUpTrainCarCardDecision::SecondAction> {
public:
  DrawSecondTrainCarCardDecisionGenerator(AsyncReader &reader,
                                          const Game &gameState);
  Ret poll() override;

private:
  AsyncReader &reader;
  const Game &gameState;
  enum struct State {
    Started,
    ReadingInput,
  } state;

  Ret started();
  Ret readingInput();
};