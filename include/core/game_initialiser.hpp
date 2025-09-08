#pragma once

#include "core/destination_ticket_card.hpp"
#include "core/game.forward.hpp"
#include "util/async_generator.hpp"
#include <array>
#include <optional>
#include <vector>

class GameInitialiser : AsyncGenerator<std::monostate> {
public:
  GameInitialiser(Game &game);
  Ret poll();
  bool done;

private:
  Game &game;
  std::vector<AsyncGeneratorPtr<uint8_t>> playerPolling;
  std::vector<std::array<DestinationTicketCard, 4>> dealtTickets;
};
