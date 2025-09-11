#include "algo/shortest_path.hpp"
#include <queue>

std::vector<uint8_t> shortestPath(const Map &map, uint8_t playerIdx,
                                  uint8_t cityIdx1, uint8_t cityIdx2) {
  using P = std::pair<int /* dist */, uint8_t /* cityIdx */>;
  auto dist = std::vector<int>(NumCities, std::numeric_limits<int>::max());
  auto q = std::priority_queue<P, std::vector<P>, std::greater<>>();
  dist[cityIdx1] = 0;
  q.push(std::make_pair(0, cityIdx1));

  auto pathToPrev = std::vector<int8_t>(NumCities, -1);

  while (!q.empty()) {
    auto [d, cityIdxFrom] = q.top();
    q.pop();
    if (d > dist[cityIdxFrom]) {
      continue;
    }

    if (cityIdxFrom == cityIdx2) {
      break;
    }

    for (auto routeIdx = 0uz; routeIdx < NumRoutes; routeIdx++) {
      auto claim = map.routes.claim[routeIdx];
      if ((map.routes.city1[routeIdx] != cityIdxFrom &&
           map.routes.city2[routeIdx] != cityIdxFrom) ||
          (claim != -1 && claim != playerIdx)) {
        continue;
      }
      auto cityIdxTo = map.routes.city1[routeIdx] == cityIdxFrom
                           ? map.routes.city2[routeIdx]
                           : map.routes.city1[routeIdx];
      // TODO: Use heuristic.
      auto weight = claim == -1 ? map.routes.length[routeIdx] : uint8_t(0);
      if (dist[cityIdxFrom] + weight >= dist[cityIdxTo]) {
        continue;
      }
      dist[cityIdxTo] = dist[cityIdxFrom] + weight;
      pathToPrev[cityIdxTo] = routeIdx;
      q.push(std::make_pair(dist[cityIdxTo], cityIdxTo));
    }
  }

  auto path = std::vector<uint8_t>();
  if (dist[cityIdx2] == std::numeric_limits<int>::max()) {
    return path;
  }

  auto cityIdxTo = cityIdx2;

  while (cityIdxTo != cityIdx1) {
    auto routeIdx = pathToPrev[cityIdxTo];
    auto cityIdxFrom = map.routes.city1[routeIdx] == cityIdxTo
                           ? map.routes.city2[routeIdx]
                           : map.routes.city1[routeIdx];
    if (map.routes.claim[routeIdx] == -1) {
      path.push_back(routeIdx);
    }
    cityIdxTo = cityIdxFrom;
  }
  return path;
}