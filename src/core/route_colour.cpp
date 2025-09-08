#include "core/route_colour.hpp"

std::string routeColourToStr(RouteColour routeColour) {
  switch (routeColour) {
  case RouteColour::Grey:
    return "Grey";
  case RouteColour::Pink:
    return "Pink";
  case RouteColour::White:
    return "White";
  case RouteColour::Blue:
    return "Blue";
  case RouteColour::Yellow:
    return "Yellow";
  case RouteColour::Orange:
    return "Orange";
  case RouteColour::Black:
    return "Black";
  case RouteColour::Red:
    return "Red";
  case RouteColour::Green:
    return "Green";
  default:
    throw std::out_of_range("RouteColour enumeration value is invalid.");
  }
}

SDL_Colour routeColourToSdlColour(RouteColour routeColour) {
  switch (routeColour) {
  case RouteColour::Grey:
    return SDL_Colour{128, 128, 128, 255};
  case RouteColour::Pink:
    return SDL_Colour{200, 50, 255, 255};
  case RouteColour::White:
    return SDL_Colour{255, 255, 255, 255};
  case RouteColour::Blue:
    return SDL_Colour{50, 210, 255, 255};
  case RouteColour::Yellow:
    return SDL_Colour{255, 245, 50, 255};
  case RouteColour::Orange:
    return SDL_Colour{255, 130, 50, 255};
  case RouteColour::Black:
    return SDL_Colour{0, 0, 0, 255};
  case RouteColour::Red:
    return SDL_Colour{255, 50, 50, 255};
  case RouteColour::Green:
    return SDL_Colour{130, 255, 50, 255};
  default:
    throw std::out_of_range("RouteColour enumeration value is invalid.");
  }
}

SDL_Colour routeColourToSdlContrastColour(RouteColour routeColour) {
  switch (routeColour) {
  case RouteColour::Grey:
    return SDL_Colour{255, 255, 255, 255};
  case RouteColour::Pink:
    return SDL_Colour{0, 0, 0, 255};
  case RouteColour::White:
    return SDL_Colour{0, 0, 0, 255};
  case RouteColour::Blue:
    return SDL_Colour{255, 255, 255, 255};
  case RouteColour::Yellow:
    return SDL_Colour{0, 0, 0, 255};
  case RouteColour::Orange:
    return SDL_Colour{0, 0, 0, 255};
  case RouteColour::Black:
    return SDL_Colour{255, 255, 255, 255};
  case RouteColour::Red:
    return SDL_Colour{0, 0, 0, 255};
  case RouteColour::Green:
    return SDL_Colour{0, 0, 0, 255};
  default:
    throw std::out_of_range("RouteColour enumeration value is invalid.");
  }
}