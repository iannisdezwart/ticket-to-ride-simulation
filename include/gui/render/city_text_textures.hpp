#include "core/map.hpp"
#include "gui/render/font.hpp"
#include "gui/render/texture.hpp"
#include <SDL2/SDL.h>
#include <array>

class CityTextTextures {
public:
  CityTextTextures(const Font &fnt, SDL_Renderer *ren);
  const Texture &operator[](uint64_t idx) const;

private:
  std::array<Texture, NumCities> tex;
};