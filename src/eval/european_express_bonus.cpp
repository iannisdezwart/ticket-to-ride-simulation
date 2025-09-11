#include "eval/european_express_bonus.hpp"

int computeLongestRoute(uint8_t playerIdx, uint8_t cityFromIdx,
                        const Routes &routes, std::bitset<NumRoutes> &vis) {
  auto max = 0;
  for (auto routeIdx = 0; routeIdx < NumRoutes; routeIdx++) {
    if (routes.claim[routeIdx] != playerIdx || vis[routeIdx] ||
        (routes.city1[routeIdx] != cityFromIdx &&
         routes.city2[routeIdx] != cityFromIdx)) {
      continue;
    }
    auto cityToIdx = routes.city1[routeIdx] == cityFromIdx
                         ? routes.city2[routeIdx]
                         : routes.city1[routeIdx];
    vis[routeIdx] = true;
    auto cand = routes.length[routeIdx] +
                computeLongestRoute(playerIdx, cityToIdx, routes, vis);
    vis[routeIdx] = false;
    max = std::max(max, cand);
  }
  return max;
}

int computeLongestRoute(uint8_t playerIdx, const Routes &routes) {
  auto max = 0;
  std::bitset<NumRoutes> vis;
  for (auto cityIdx = uint8_t(0); cityIdx < NumCities; cityIdx++) {
    max = std::max(max, computeLongestRoute(playerIdx, cityIdx, routes, vis));
  }
  return max;
}

std::vector<int> evalEuropeanExpressBonus(uint8_t numPlayers,
                                                const Routes &routes) {
  auto ret = std::vector<int>(numPlayers);
  for (auto playerIdx = uint8_t(0); playerIdx < numPlayers; playerIdx++) {
    ret[playerIdx] = computeLongestRoute(playerIdx, routes);
  }
  auto max = *std::max_element(ret.begin(), ret.end());
  std::transform(ret.begin(), ret.end(), ret.begin(),
                 [max](const auto &x) { return x == max ? 10 : 0; });
  return ret;
}