#include "core/train_car_card_hand.hpp"
#include <sstream>

void TrainCarCardHand::add(TrainCarCard card) {
  // clang-format off
  switch (card) {
  case TrainCarCard::Pink:        numPink++;        break;
  case TrainCarCard::White:       numWhite++;       break;
  case TrainCarCard::Blue:        numBlue++;        break;
  case TrainCarCard::Yellow:      numYellow++;      break;
  case TrainCarCard::Orange:      numOrange++;      break;
  case TrainCarCard::Black:       numBlack++;       break;
  case TrainCarCard::Red:         numRed++;         break;
  case TrainCarCard::Green:       numGreen++;       break;
  case TrainCarCard::Locomotive:  numLocomotive++;  break;
  }
  // clang-format on
}

void TrainCarCardHand::remove(TrainCarCard card) {
  switch (card) {
  case TrainCarCard::Pink:
    if (numPink == 0) {
      throw std::logic_error("Player doesn't have any pink train car cards.");
    }
    numPink--;
    break;
  case TrainCarCard::White:
    if (numWhite == 0) {
      throw std::logic_error("Player doesn't have any white train car cards.");
    }
    numWhite--;
    break;
  case TrainCarCard::Blue:
    if (numBlue == 0) {
      throw std::logic_error("Player doesn't have any blue train car cards.");
    }
    numBlue--;
    break;
  case TrainCarCard::Yellow:
    if (numYellow == 0) {
      throw std::logic_error("Player doesn't have any yellow train car cards.");
    }
    numYellow--;
    break;
  case TrainCarCard::Orange:
    if (numOrange == 0) {
      throw std::logic_error("Player doesn't have any orange train car cards.");
    }
    numOrange--;
    break;
  case TrainCarCard::Black:
    if (numBlack == 0) {
      throw std::logic_error("Player doesn't have any black train car cards.");
    }
    numBlack--;
    break;
  case TrainCarCard::Red:
    if (numRed == 0) {
      throw std::logic_error("Player doesn't have any red train car cards.");
    }
    numRed--;
    break;
  case TrainCarCard::Green:
    if (numGreen == 0) {
      throw std::logic_error("Player doesn't have any green train car cards.");
    }
    numGreen--;
    break;
  case TrainCarCard::Locomotive:
    if (numLocomotive == 0) {
      throw std::logic_error(
          "Player doesn't have any locomotive train car cards.");
    }
    numLocomotive--;
    break;
  }
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
  if (numPink > 0)        oss << "P x"   << numPink        << ", ";
  if (numWhite > 0)       oss << "W x "  << numWhite       << ", ";
  if (numBlue > 0)        oss << "U x "  << numBlue        << ", ";
  if (numYellow > 0)      oss << "Y x "  << numYellow      << ", ";
  if (numOrange > 0)      oss << "O x"   << numOrange      << ", ";
  if (numBlack > 0)       oss << "B x"   << numBlack       << ", ";
  if (numRed > 0)         oss << "R x"   << numRed         << ", ";
  if (numGreen > 0)       oss << "G x"   << numGreen       << ", ";
  if (numLocomotive > 0)  oss << "L x "  << numLocomotive  << ", ";
  // clang-format on
  oss << '\n';
  return oss.str();
}