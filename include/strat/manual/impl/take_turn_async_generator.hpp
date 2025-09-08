#pragma once

#include "core/game.hpp"
#include "turn/turn_decision.hpp"
#include "util/async_generator.hpp"
#include "util/async_reader.hpp"

class TakeTurnAsyncGenerator : public AsyncGenerator<TurnDecision> {
public:
  TakeTurnAsyncGenerator(AsyncReader &reader, const Game &gameState);
  Ret poll() override;

private:
  AsyncReader &reader;
  const Game &gameState;
  enum struct State {
    Started,
    ReadingInput,
    WaitingForChild,
  } state;
  AsyncGeneratorPtr<TurnDecision> child;

  Ret started();
  Ret readingInput();
  Ret waitingForChild();

  Ret drawTrainCarCards();
  Ret claimRoute();
  Ret buildTrainStation();
};