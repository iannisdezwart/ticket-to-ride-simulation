#include "core/train_car_card.hpp"

std::string trainCarCardToStr(TrainCarCard card) {
  // clang-format off
  switch (card) {
  case TrainCarCard::Pink:        return "Pink";
  case TrainCarCard::White:       return "White";
  case TrainCarCard::Blue:        return "Blue";
  case TrainCarCard::Yellow:      return "Yellow";
  case TrainCarCard::Orange:      return "Orange";
  case TrainCarCard::Black:       return "Black";
  case TrainCarCard::Red:         return "Red";
  case TrainCarCard::Green:       return "Green";
  case TrainCarCard::Locomotive:  return "Locomotive";
  default:
    throw std::out_of_range("TrainCarCard enumeration value is invalid.");
  }
  // clang-format on
}

TrainCarCard trainCarCardFromChar(char c) {
  // clang-format off
  switch (c) {
  case 'P': return TrainCarCard::Pink;
  case 'W': return TrainCarCard::White;
  case 'U': return TrainCarCard::Blue;
  case 'Y': return TrainCarCard::Yellow;
  case 'O': return TrainCarCard::Orange;
  case 'B': return TrainCarCard::Black;
  case 'R': return TrainCarCard::Red;
  case 'G': return TrainCarCard::Green;
  case 'L': return TrainCarCard::Locomotive;
  default: throw std::out_of_range("Invalid TrainCarCard character");
  }
  // clang-format on
}

std::vector<TrainCarCard> parsePaymentString(const std::string &str) {
  auto res = std::vector<TrainCarCard>(str.size());
  for (auto i = 0uz; i < str.size(); i++) {
    res[i] = trainCarCardFromChar(str[i]);
  }
  return res;
}
