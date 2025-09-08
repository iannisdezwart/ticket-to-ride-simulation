#pragma once

#include <vector>
#include "core/train_car_card.hpp"

struct ClaimRouteDecision {
    uint8_t routeIdx;
    std::vector<TrainCarCard> payment;
    std::vector<TrainCarCard> tunnelBackupPayment;
};