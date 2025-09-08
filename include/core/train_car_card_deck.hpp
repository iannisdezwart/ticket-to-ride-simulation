#pragma once

#include <algorithm>
#include <span>
#include <utility>
#include <vector>
#include "core/train_car_card.hpp"
#include "util/rng.hpp"

constexpr const int NumTrainCarCards = 110;

struct TrainCarCardDeck {
  // Face up: [0, 4].
  // Face down pile: [5, N).
  // Drawn cards: [N, M). Don't read from here.
  // Discard pile: [M, 110].
  std::array<TrainCarCard, NumTrainCarCards> cards;
  uint8_t n;
  uint8_t m;

  TrainCarCardDeck() noexcept;

  TrainCarCard drawFromFaceDownPile();
  TrainCarCard takeFromFaceUpPile(uint8_t idx);
  void discard(TrainCarCard card);
  void shuffleDiscardPileIntoFaceDownPile();
  std::span<const TrainCarCard> getFaceUpCards() const;
};