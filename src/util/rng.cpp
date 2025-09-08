#include "util/rng.hpp"

static auto gen = std::mt19937(std::random_device()());
std::mt19937 &rng() { return gen; }
