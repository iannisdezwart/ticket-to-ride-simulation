#pragma once

#include "core/game.hpp"
#include "turn/turn_decision.hpp"
#include "util/async_generator.hpp"
#include "util/async_reader.hpp"
#include "strat/manual/impl/payment_async_generator.hpp"

class ClaimRouteDecisionGenerator : public AsyncGenerator<ClaimRouteDecision> {
public:
  ClaimRouteDecisionGenerator(AsyncReader &reader, const Game &gameState);
  Ret poll() override;

private:
  AsyncReader &reader;
  const Game &gameState;
  enum struct State {
    Started,
    ReadingRouteInput,
    WaitingForPayment,
    WaitingForTunnelBackupPayment,
  } state;
  uint8_t routeIdx;
  std::vector<TrainCarCard> payment;
  std::vector<TrainCarCard> tunnelBackupPayment;
  std::unique_ptr<PaymentAsyncGenerator> paymentGenerator;

  Ret started();
  Ret readingRouteInput();
  Ret waitingForPayment();
  Ret waitingForTunnelBackupPayment();
  Ret finish();
};