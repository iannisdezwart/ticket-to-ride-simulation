#pragma once

#include "eval/completed_destination_tickets.hpp"

struct PlayerReport {
  uint8_t playerIdx;
  int totalPoints;
  int completedRoutesPoints;
  CompletedDestinationTicketsReport completedDestinationTickets;
  int unplayedTrainStationPoints;
  int europeanExpressBonusPoints;
};