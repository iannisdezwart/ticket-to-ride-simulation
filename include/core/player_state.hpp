#pragma once

#include <cstdint>
#include <vector>
#include "core/destination_ticket_card_deck.hpp"
#include "core/train_car_card_deck.hpp"
#include "core/train_car_card_hand.hpp"

struct PlayerState {
  uint8_t trainCars;
  uint8_t trainStations;
  TrainCarCardHand trainCarCards;
  std::vector<DestinationTicketCard> destinationTicketCards;

  PlayerState() = default;
  PlayerState(TrainCarCardDeck &trainCarCardDeck);
};