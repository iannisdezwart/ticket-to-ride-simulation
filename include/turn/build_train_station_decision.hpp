#pragma once

#include <vector>
#include "core/train_car_card.hpp"

struct BuildTrainStationDecision {
    uint8_t cityIdx;
    std::vector<TrainCarCard> payment;
};