#pragma once

#include <SDL2/SDL_ttf.h>

class Font {
public:
  Font(TTF_Font *fnt);
  Font(const Font &) = delete;
  Font &operator=(const Font &) = delete;
  Font(Font &&f);
  Font &operator=(Font &&f);
  ~Font();
  TTF_Font *get() const;
  static Font open(const char *path, uint16_t size);

private:
  TTF_Font *fnt;
};