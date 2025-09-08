#pragma once

#include "core/destination_ticket_card.hpp"
#include "core/map.hpp"
#include <vector>

struct CompletedRoutesReport {
  std::vector<bool> completed;
  int points;
};

CompletedRoutesReport
evalCompletedRoutes(int playerIdx, const Map &map,
                    std::vector<DestinationTicketCard> cards);