#include "gui/render/font.hpp"

Font::Font(TTF_Font *fnt) : fnt(fnt) {}
Font::~Font() { TTF_CloseFont(fnt); }
Font::Font(Font &&f) {
  fnt = f.fnt;
  f.fnt = nullptr;
}
Font &Font::operator=(Font &&f) {
  if (this == &f) {
    return *this;
  }
  fnt = f.fnt;
  f.fnt = nullptr;
  return *this;
}
TTF_Font *Font::get() const { return fnt; }

Font Font::open(const char *path, uint16_t size) {
  auto font = TTF_OpenFont(path, size);
  if (font == nullptr) {
    SDL_Log("TTF_OpenFont(): %s", TTF_GetError());
  }
  return Font(font);
}