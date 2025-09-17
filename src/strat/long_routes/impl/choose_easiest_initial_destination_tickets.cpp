#include "strat/long_routes/impl/choose_easiest_initial_destination_tickets.hpp"
#include "algo/steiner_forest.hpp"
#include <numeric>

int computeTotalLength(const Map &map, const std::vector<uint8_t> &routeIdxs) {
  return std::accumulate(
      routeIdxs.begin(), routeIdxs.end(), 0,
      [&map](int sum, uint8_t idx) { return sum + map.routes.length[idx]; });
}

uint8_t chooseEasiestInitialDestinationTickets(
    const Map &map, const std::array<DestinationTicketCard, 4> &cards) {
  auto lowestTotalLength = std::numeric_limits<int>::max();
  auto mask = uint8_t();
  for (auto i = 0; i < 4; i++) {
    for (auto j = 0; j < 4; j++) {
      if (i == j) {
        continue;
      }
      auto paths = std::vector<std::pair<uint8_t, uint8_t>>{
          {cards[i].city1, cards[i].city2},
          {cards[j].city1, cards[j].city2},
      };
      auto routes = steinerForest(map, 0, paths);
      auto totalLength = computeTotalLength(map, routes);
      if (totalLength < lowestTotalLength) {
        lowestTotalLength = totalLength;
        mask = (1 << i) | (1 << j);
      }
    }
  }
  return mask;
}