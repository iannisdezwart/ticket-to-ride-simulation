#pragma once

#include "core/game.hpp"
#include "turn/turn_decision.hpp"
#include "util/async_generator.hpp"
#include "util/async_reader.hpp"

class DrawTrainCarCardsDecisionGenerator
    : public AsyncGenerator<DrawTrainCarCardsDecision> {
public:
  DrawTrainCarCardsDecisionGenerator(AsyncReader &reader,
                                     const Game &gameState);
  Ret poll() override;

private:
  AsyncReader &reader;
  const Game &gameState;
  enum struct State {
    Started,
    ReadingDecisionInput,
    ReadingDrawOneFaceUpDecisionInput,
  } state;

  Ret started();
  Ret readingDecisionInput();
  Ret drawFaceUpTrainCarCardDecision();
  Ret readingDrawOneFaceUpDecisionInput();
};