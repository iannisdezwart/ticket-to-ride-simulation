#pragma once

#include "core/destination_ticket_card.hpp"
#include "core/game.hpp"
#include "turn/turn_decision.hpp"
#include "util/async_generator.hpp"

struct PlayerStrategy {
  virtual ~PlayerStrategy() = default;

  // Generates a bit mask of chosen routes.
  // 0bxxxxDCBA where A, B, C, D are booleans indicating whether or not to keep
  // the corresponding route in the input array.
  virtual AsyncGeneratorPtr<uint8_t> chooseInitialDestinationTickets(
      std::array<DestinationTicketCard, 4> cards) = 0;

  // Called each turn. Makes a decision for what to do this turn.
  virtual AsyncGeneratorPtr<TurnDecision> takeTurn(const Game &gameState) = 0;
};