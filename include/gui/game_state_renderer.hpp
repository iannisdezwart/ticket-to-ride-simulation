#pragma once

#include "core/game.hpp"
#include "gui/render/city_text_textures.hpp"
#include "gui/render/texture.hpp"
#include <SDL2/SDL.h>

class GameStateRenderer {
public:
  GameStateRenderer(SDL_Renderer *ren, const Game &game);
  ~GameStateRenderer();
  void render();

private:
  SDL_Renderer *ren;
  const Game &game;
  Texture mapTex;
  Texture cityMarkerTex;
  Font bebasNeueFont;
  CityTextTextures cityTextTex;

  void renderMap();
  void renderRoutes();
  void renderRoutesLines(const std::array<std::pair<int, int>, NumRoutes> &from,
                         const std::array<std::pair<int, int>, NumRoutes> &to);
  void
  renderRoutesMarkers(const std::array<std::pair<int, int>, NumRoutes> &mid);
  void renderRouteMarkerTunnelBorder(int x, int y, SDL_Colour colour);
  void renderRouteMarkerBox(int x, int y, SDL_Colour colour);
  void condRenderFerryMarkers(uint8_t numFerries, int x, int y,
                              SDL_Colour colour);
  void renderRouteMarkerLabelText(int x, int y, uint8_t text,
                                  SDL_Colour colour);
  void renderCities();
  void renderPlayerState();
  void renderPlayerStatePlayerIdx();
  void renderPlayerStateInventory();
  void renderPlayerStateTrainCarCardsHand();
  void renderPlayerStateDestinationTickets();
  void renderFaceUpTrainCarCards();
};
