#pragma once

#include <cstdint>
#include <string>
#include <SDL2/SDL.h>

enum struct RouteColour : uint8_t {
  Grey,
  Pink,
  White,
  Blue,
  Yellow,
  Orange,
  Black,
  Red,
  Green,
};

std::string routeColourToStr(RouteColour routeColour);
SDL_Colour routeColourToSdlColour(RouteColour routeColour);
SDL_Colour routeColourToSdlContrastColour(RouteColour routeColour);
