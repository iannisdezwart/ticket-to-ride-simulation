#include "eval/completed_routes.hpp"
#include <stdexcept>

int pointsForCompletedRoute(uint8_t length) {
  // clang-format off
  switch (length) {
  case 1:  return 1;
  case 2:  return 2;
  case 3:  return 4;
  case 4:  return 7;
  case 6:  return 15;
  case 8:  return 21;
  default: throw std::logic_error("Unhandled route length.");
  }
  // clang-format on
}

std::vector<int> evalCompletedRoutes(uint8_t numPlayers, const Routes &routes) {
  auto ret = std::vector<int>(numPlayers);
  for (auto routeIdx = 0; routeIdx < NumRoutes; routeIdx++) {
    auto claim = routes.claim[routeIdx];
    auto length = routes.length[routeIdx];
    if (claim != -1) {
      ret[claim] += pointsForCompletedRoute(length);
    }
  }
  return ret;
}
