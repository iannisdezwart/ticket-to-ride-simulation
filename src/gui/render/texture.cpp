#include "gui/render/texture.hpp"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

Texture::Texture(SDL_Texture *tex) : tex(tex) {}
Texture::~Texture() { SDL_DestroyTexture(tex); }
Texture::Texture(Texture &&t) {
  tex = t.tex;
  t.tex = nullptr;
}
Texture &Texture::operator=(Texture &&t) {
  if (this == &t) {
    return *this;
  }
  tex = t.tex;
  t.tex = nullptr;
  return *this;
}
SDL_Texture *Texture::get() const { return tex; }

std::pair<int, int> Texture::dim() const {
  auto ret = std::pair<int, int>();
  SDL_QueryTexture(tex, nullptr, nullptr, &ret.first, &ret.second);
  return ret;
}

Texture Texture::img(const char *sv, SDL_Renderer *ren) {
  auto sfc = IMG_Load(sv);
  if (sfc == nullptr) {
    SDL_Log("IMG_Load(): %s", IMG_GetError());
    return Texture();
  }
  auto tex = SDL_CreateTextureFromSurface(ren, sfc);
  SDL_FreeSurface(sfc);
  if (tex == nullptr) {
    SDL_Log("SDL_CreateTextureFromSurface: %s", SDL_GetError());
  }
  return Texture(tex);
}

Texture Texture::text(const Font &fnt, const char *text, SDL_Colour colour,
                      SDL_Renderer *ren) {
  auto sfc = TTF_RenderUTF8_Blended(fnt.get(), text, colour);
  if (sfc == nullptr) {
    SDL_Log("TTF_RenderText_Solid(): %s", TTF_GetError());
    return Texture();
  }
  auto tex = SDL_CreateTextureFromSurface(ren, sfc);
  SDL_FreeSurface(sfc);
  if (tex == nullptr) {
    SDL_Log("SDL_CreateTextureFromSurface: %s", SDL_GetError());
  }
  return Texture(tex);
}