#include "core/destination_ticket_card.hpp"
#include "core/map.hpp"
#include <cstdint>

uint8_t chooseEasiestInitialDestinationTickets(
    const Map &map, const std::array<DestinationTicketCard, 4> &cards);