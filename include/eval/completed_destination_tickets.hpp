#pragma once

#include "core/destination_ticket_card.hpp"
#include "core/game.forward.hpp"
#include "core/map.hpp"
#include <vector>

struct CompletedDestinationTicketsReport {
  std::vector<bool> completed;
  int points;
};

CompletedDestinationTicketsReport evalCompletedDestinationTickets(
    int playerIdx, const Map &map,
    const std::vector<DestinationTicketCard> &cards);

CompletedDestinationTicketsReport
evalCompletedDestinationTickets(const Game &game);