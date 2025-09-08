#include "core/map.hpp"

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