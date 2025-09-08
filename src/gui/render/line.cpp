#include "gui/render/line.hpp"
#include <SDL2/SDL2_gfxPrimitives.h>
#include <array>
#include <cmath>

void drawLine(SDL_Renderer *ren, int xFrom, int yFrom, int xTo, int yTo,
              int thickness, SDL_Colour colour) {
  thickLineRGBA(ren, xFrom, yFrom, xTo, yTo, thickness, colour.r, colour.g,
                colour.b, colour.a);
}