#pragma once

#include "core/game.hpp"

void startGui(const Game &game);

// Returns true when the GUI is still running.
bool runGui();

void destroyGui();