#pragma once

#include "core/map.hpp"
#include <cstdint>
#include <vector>

int computeLongestRoute(uint8_t playerIdx, const Routes &routes);
std::vector<int> evalEuropeanExpressBonus(uint8_t numPlayers,
                                          const Routes &routes);