#include "core/player_state.hpp"

PlayerState::PlayerState(TrainCarCardDeck &trainCarCardDeck)
    : trainCars(45), trainStations(3) {
  for (auto i = 0u; i < 4u; i++) {
    trainCarCards.add(trainCarCardDeck.drawFromFaceDownPile());
  }
}