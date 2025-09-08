#pragma once

#include <variant>
#include "turn/build_train_station_decision.hpp"
#include "turn/claim_route_decision.hpp"
#include "turn/draw_destination_tickets_decision.hpp"
#include "turn/draw_train_car_cards_decision.hpp"

using TurnDecision =
    std::variant<DrawTrainCarCardsDecision, ClaimRouteDecision,
                 DrawDestinationTicketsDecision, BuildTrainStationDecision>;
