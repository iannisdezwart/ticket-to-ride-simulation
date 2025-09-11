#include "core/map.hpp"
#include <sstream>

Map::Map() {
  for (auto i = 0; i < NumRoutes; i++) {
    routes.city1[i] = standardRoutes[i].city1;
    routes.city2[i] = standardRoutes[i].city2;
    routes.colour[i] = standardRoutes[i].colour;
    routes.length[i] = standardRoutes[i].length;
    routes.numFerries[i] = standardRoutes[i].numFerries;
    routes.isTunnel[i] = standardRoutes[i].isTunnel;
    routes.parallelism[i] = standardRoutes[i].parallelism;
    routes.claim[i] = -1;
  }
  for (auto i = 0; i < NumCities; i++) {
    trainStationClaims[i] = -1;
  }
}

uint8_t Map::routeIdx(const char *city1, const char *city2) {
  for (auto i = 0; i < NumRoutes; i++) {
    auto c1 = standardCities[routes.city1[i]];
    auto c2 = standardCities[routes.city2[i]];
    if ((city1 == c1 && city2 == c2) || (city1 == c2 && city2 == c1)) {
      return uint8_t(i);
    }
  }

  std::ostringstream oss;
  oss << "No route found between " << city1 << " and " << city2;
  throw std::out_of_range(oss.str());
}
