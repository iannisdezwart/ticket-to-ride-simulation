#pragma once

#include "core/destination_ticket_card.hpp"
#include "core/game.hpp"
#include "turn/turn_decision.hpp"
#include "util/async_generator.hpp"

TurnDecision focusOnUncompletedRoute(const Game &gameState,
                                     const DestinationTicketCard &card);