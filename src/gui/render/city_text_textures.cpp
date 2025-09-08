#include "gui/render/city_text_textures.hpp"

CityTextTextures::CityTextTextures(const Font &fnt, SDL_Renderer *ren) {
  for (auto i = 0uz; i < standardCities.size(); i++) {
    const auto &cityName = standardCities[i];
    tex[i] = Texture::text(fnt, cityName, {129, 69, 19, 255}, ren);
  }
}

const Texture &CityTextTextures::operator[](uint64_t idx) const {
  return tex[idx];
}