#pragma once

#include <variant>
#include <functional>
#include "util/async_generator.hpp"

struct DrawTwoFaceDownTrainCarCardsDecision {};
struct DrawOneFaceUpLocomotiveCardDecision {};

struct DrawOneFaceUpTrainCarCardDecision {
  struct DrawSecondFaceUpTrainCarCardDecision {
    uint8_t idx;
  };
  struct DrawSecondFaceDownTrainCarCardDecision {};
  using SecondAction = std::variant<DrawSecondFaceUpTrainCarCardDecision,
                                    DrawSecondFaceDownTrainCarCardDecision>;

  uint8_t idx;
  std::function<AsyncGeneratorPtr<SecondAction>()> secondAction;
};

using DrawTrainCarCardsDecision =
    std::variant<DrawTwoFaceDownTrainCarCardsDecision,
                 DrawOneFaceUpLocomotiveCardDecision,
                 DrawOneFaceUpTrainCarCardDecision>;
