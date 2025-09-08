#pragma once

#include "core/train_car_card.hpp"
#include <cstdint>

struct TrainCarCardHand {
  uint8_t numPink{0};
  uint8_t numWhite{0};
  uint8_t numBlue{0};
  uint8_t numYellow{0};
  uint8_t numOrange{0};
  uint8_t numBlack{0};
  uint8_t numRed{0};
  uint8_t numGreen{0};
  uint8_t numLocomotive{0};

  void add(TrainCarCard card);
  void remove(TrainCarCard card);
  bool subset(const TrainCarCardHand &h) const;
  std::string report() const;
};