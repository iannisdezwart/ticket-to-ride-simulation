#pragma once

#include <SDL2/SDL.h>
#include <stdexcept>

SDL_Color playerIdxToSdlColour(uint8_t idx) {
  // clang-format off
  switch (idx ) {
  case 0: return SDL_Colour{  0,   0, 230, 255};
  case 1: return SDL_Colour{230,   0,   0, 255};
  case 2: return SDL_Colour{  0, 230,   0, 255};
  case 3: return SDL_Colour{230, 230,   0, 255};
  case 4: return SDL_Colour{  0,   0,   0, 255};
  default: throw std::range_error("Player index out of range.");
  }
  // clang-format on
}