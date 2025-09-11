#pragma once

#include "core/map.hpp"

// Returns indexes to `Map::routes`.
std::vector<uint8_t> shortestPath(const Map &map, uint8_t playerIdx,
                                  uint8_t cityIdx1, uint8_t cityIdx2);