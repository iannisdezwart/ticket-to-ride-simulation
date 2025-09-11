#pragma once

#include "core/train_car_card.hpp"
#include <cstdint>

class TrainCarCardHand {
public:
  const uint8_t &count(TrainCarCard card) const;
  uint8_t &count(TrainCarCard card);
  void add(TrainCarCard card);
  void remove(TrainCarCard card);
  bool subset(const TrainCarCardHand &h) const;
  std::string report() const;

private:
  uint8_t numPink{0};
  uint8_t numWhite{0};
  uint8_t numBlue{0};
  uint8_t numYellow{0};
  uint8_t numOrange{0};
  uint8_t numBlack{0};
  uint8_t numRed{0};
  uint8_t numGreen{0};
  uint8_t numLocomotive{0};

  template <typename Self> static auto &access(Self &self, TrainCarCard card) {
    // clang-format off
    switch (card) {
    case TrainCarCard::Pink:        return self.numPink;
    case TrainCarCard::White:       return self.numWhite;
    case TrainCarCard::Blue:        return self.numBlue;
    case TrainCarCard::Yellow:      return self.numYellow;
    case TrainCarCard::Orange:      return self.numOrange;
    case TrainCarCard::Black:       return self.numBlack;
    case TrainCarCard::Red:         return self.numRed;
    case TrainCarCard::Green:       return self.numGreen;
    case TrainCarCard::Locomotive:  return self.numLocomotive;
    default: throw std::logic_error("Invalid enumeration value.");
    }
    // clang-format on
  }
};