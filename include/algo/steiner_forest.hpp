#include "core/map.hpp"
#include <cstdint>
#include <vector>

// Returns indexes to `Map::routes`.
std::vector<uint8_t>
steinerForest(const Map &map, uint8_t playerIdx,
              std::vector<std::pair<uint8_t, uint8_t>> &paths);