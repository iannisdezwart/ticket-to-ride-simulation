#pragma once

#include "gui/render/font.hpp"
#include <SDL2/SDL.h>
#include <string_view>

class Texture {
public:
  Texture() = default;
  Texture(const Texture &) = delete;
  Texture &operator=(const Texture &) = delete;
  Texture(Texture &&t);
  Texture &operator=(Texture &&t);
  Texture(SDL_Texture *tex);
  ~Texture();
  SDL_Texture *get() const;
  std::pair<int, int> dim() const;
  static Texture img(const char *sv, SDL_Renderer *ren);
  static Texture text(const Font &fnt, const char *text, SDL_Colour colour,
                      SDL_Renderer *ren);

private:
  SDL_Texture *tex;
};