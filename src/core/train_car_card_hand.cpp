#include "core/train_car_card_hand.hpp"
#include <sstream>

uint8_t &TrainCarCardHand::count(TrainCarCard card) {
  return access(*this, card);
}

const uint8_t &TrainCarCardHand::count(TrainCarCard card) const {
  return access(*this, card);
}

void TrainCarCardHand::add(TrainCarCard card) { count(card)++; }

void TrainCarCardHand::remove(TrainCarCard card) {
  auto &cnt = count(card);
  if (cnt == 0) {
    std::ostringstream oss;
    oss << "Player doesn't have any " << trainCarCardToStr(card)
        << " train car cards.";
    throw std::logic_error(oss.str());
  }
  cnt--;
}

bool TrainCarCardHand::subset(const TrainCarCardHand &h) const {
  return numPink <= h.numPink && numWhite <= h.numWhite &&
         numBlue <= h.numBlue && numYellow <= h.numYellow &&
         numOrange <= h.numOrange && numBlack <= h.numBlack &&
         numRed <= h.numRed && numGreen <= h.numGreen &&
         numLocomotive <= h.numLocomotive;
}

std::string TrainCarCardHand::report() const {
  std::ostringstream oss;
  // clang-format off
  if (numPink > 0)        oss << "P x"   << int(numPink)        << ", ";
  if (numWhite > 0)       oss << "W x "  << int(numWhite)       << ", ";
  if (numBlue > 0)        oss << "U x "  << int(numBlue)        << ", ";
  if (numYellow > 0)      oss << "Y x "  << int(numYellow)      << ", ";
  if (numOrange > 0)      oss << "O x"   << int(numOrange)      << ", ";
  if (numBlack > 0)       oss << "B x"   << int(numBlack)       << ", ";
  if (numRed > 0)         oss << "R x"   << int(numRed)         << ", ";
  if (numGreen > 0)       oss << "G x"   << int(numGreen)       << ", ";
  if (numLocomotive > 0)  oss << "L x "  << int(numLocomotive)  << ", ";
  // clang-format on
  oss << '\n';
  return oss.str();
}