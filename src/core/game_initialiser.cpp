#include "core/game_initialiser.hpp"
#include "core/game.hpp"
#include "strat/player_strategy.hpp"

GameInitialiser::GameInitialiser(Game &game)
    : done(false), game(game), dealtTickets(game.playerStrategies.size()) {
  playerPolling.reserve(game.playerStrategies.size());
  for (auto i = 0uz; i < game.playerStrategies.size(); i++) {
    auto &strat = game.playerStrategies[i];
    dealtTickets[i] = std::array<DestinationTicketCard, 4>{
        game.destinationTicketCardDeck.drawLongCard(),
        game.destinationTicketCardDeck.drawRegularCard(),
        game.destinationTicketCardDeck.drawRegularCard(),
        game.destinationTicketCardDeck.drawRegularCard(),
    };
    playerPolling.push_back(
        strat->chooseInitialDestinationTickets(dealtTickets[i]));
  }
}

GameInitialiser::Ret GameInitialiser::poll() {
  done = true;
  for (auto i = 0uz; i < playerPolling.size(); i++) {
    if (!playerPolling[i]) {
      continue;
    }

    auto res = playerPolling[i]->poll();
    if (!res.has_value()) {
      done = false;
      continue;
    }

    playerPolling[i] = nullptr;
    auto mask = res.value();
    if (std::popcount(mask) < 2) {
      throw std::range_error("Less than 2 destination tickets were chosen.");
    }
    for (auto j = 0uz; j < dealtTickets[i].size(); j++) {
      const auto card = dealtTickets[i][j];
      if (mask & (1 << j)) {
        game.playerStates[i].destinationTicketCards.push_back(card);
      } else if (card.points < 20) {
        game.destinationTicketCardDeck.returnRegularCard(card);
      }
    }
  }

  game.handleThreeLocomotivesInFaceUpPile();
  return done ? std::make_optional<std::monostate>() : std::nullopt;
}