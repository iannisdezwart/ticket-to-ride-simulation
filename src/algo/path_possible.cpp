#include "algo/path_possible.hpp"

bool dfs(const Map &map, uint8_t playerIdx, uint8_t cityFrom, uint8_t cityTo,
            std::bitset<NumCities> &vis) {
  if (cityFrom == cityTo) {
    return true;
  }

  for (auto routeIdx = 0; routeIdx < NumRoutes; routeIdx++) {
    auto claim = map.routes.claim[routeIdx];
    if (claim != -1 && claim != playerIdx) {
      continue;
    }
    auto city1 = map.routes.city1[routeIdx];
    auto city2 = map.routes.city2[routeIdx];
    if (city1 != cityFrom && city2 != cityFrom) {
      continue;
    }
    auto newCity = city1 == cityFrom ? city2 : city1;
    if (vis[newCity]) {
      continue;
    }
    vis[newCity] = true;
    if (dfs(map, playerIdx, newCity, cityTo, vis)) {
      return true;
    }
  }

  return false;
}

bool pathPossible(const Map &map, uint8_t playerIdx, uint8_t cityIdx1,
                  uint8_t cityIdx2) {
  auto vis = std::bitset<NumCities>();
  return dfs(map, playerIdx, cityIdx1, cityIdx2, vis);
}