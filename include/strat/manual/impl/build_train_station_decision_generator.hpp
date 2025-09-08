#pragma once

#include "core/game.hpp"
#include "strat/manual/impl/payment_async_generator.hpp"
#include "turn/turn_decision.hpp"
#include "util/async_generator.hpp"
#include "util/async_reader.hpp"

class BuildTrainStationDecisionGenerator
    : public AsyncGenerator<BuildTrainStationDecision> {
public:
  BuildTrainStationDecisionGenerator(AsyncReader &reader,
                                     const Game &gameState);
  Ret poll() override;

private:
  AsyncReader &reader;
  const Game &gameState;
  enum struct State {
    Started,
    ReadingCityInput,
    WaitingForPayment,
  } state;
  uint8_t cityIdx;
  std::vector<TrainCarCard> payment;
  std::unique_ptr<PaymentAsyncGenerator> paymentGenerator;

  Ret started();
  Ret readingCityInput();
  Ret waitingForPayment();
};