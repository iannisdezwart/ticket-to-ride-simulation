#pragma once

#include "core/map.hpp"
#include "core/train_car_card.hpp"
#include "core/train_car_card_hand.hpp"
#include <cstdint>
#include <optional>
#include <vector>

struct RoutePaymentOption {
  std::vector<TrainCarCard> payment;
  std::vector<TrainCarCard> tunnelBackupPayment;
};

std::vector<RoutePaymentOption>
routePaymentOptions(const TrainCarCardHand &hand, const Map &map,
                    uint8_t routeIdx);
