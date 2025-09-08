#include "core/train_car_card_deck.hpp"

TrainCarCardDeck::TrainCarCardDeck() noexcept {
  auto it = cards.begin();
  std::fill(it, it + 12, TrainCarCard::Pink);
  std::fill(it + 12, it + 24, TrainCarCard::White);
  std::fill(it + 24, it + 36, TrainCarCard::Blue);
  std::fill(it + 36, it + 48, TrainCarCard::Yellow);
  std::fill(it + 48, it + 60, TrainCarCard::Orange);
  std::fill(it + 60, it + 72, TrainCarCard::Black);
  std::fill(it + 72, it + 84, TrainCarCard::Red);
  std::fill(it + 84, it + 96, TrainCarCard::Green);
  std::fill(it + 96, it + 110, TrainCarCard::Locomotive);
  n = NumTrainCarCards;
  m = NumTrainCarCards;
  std::shuffle(cards.begin(), cards.end(), rng());
}

TrainCarCard TrainCarCardDeck::drawFromFaceDownPile() {
  if (n <= 5) {
    if (m <= 5) {
      throw std::runtime_error("Draw and discard piles are empty.");
    }
    shuffleDiscardPileIntoFaceDownPile();
  }
  n--;
  return cards[n];
}

TrainCarCard TrainCarCardDeck::takeFromFaceUpPile(uint8_t idx) {
  if (idx >= 5) {
    throw std::out_of_range("Index out of range for face up pile");
  }
  n--;
  std::swap(cards[idx], cards[n]);
  return cards[n];
}

void TrainCarCardDeck::discard(TrainCarCard card) {
  if (m <= n) {
    throw std::logic_error("More cards discarded than drawn");
  }
  m--;
  cards[m] = card;
}

void TrainCarCardDeck::shuffleDiscardPileIntoFaceDownPile() {
  std::copy(cards.begin() + m, cards.end(), cards.begin() + n);
  n = m;
  m = NumTrainCarCards;
  std::shuffle(cards.begin() + 5, cards.begin() + n, rng());
}

std::span<const TrainCarCard> TrainCarCardDeck::getFaceUpCards() const {
  return std::span(cards.data(), 5);
}