#pragma once

#include "core/destination_ticket_card.hpp"
#include "util/async_generator.hpp"
#include <array>
#include <cstdint>
#include <functional>

struct DrawDestinationTicketsDecision {
  // Generates bit mask of chosen routes.
  // 0bxxxxDCBA where A, B, C are booleans indicating whether or not to keep
  // the corresponding route in the input array.
  std::function<AsyncGeneratorPtr<uint8_t>(
      std::array<DestinationTicketCard, 3>)>
      chooseToKeep;
};