#include "gui/game_state_renderer.hpp"
#include "core/player_colour.hpp"
#include "gui/render/line.hpp"
#include "gui/render/texture.hpp"
#include <SDL2/SDL2_gfxPrimitives.h>
#include <cmath>
#include <sstream>

GameStateRenderer::GameStateRenderer(SDL_Renderer *ren, const Game &game)
    : ren(ren), game(game), mapTex(Texture::img("assets/map.png", ren)),
      cityMarkerTex(Texture::img("assets/city_marker.png", ren)),
      bebasNeueFont(Font::open("assets/bebas_neue.ttf", 20)),
      cityTextTex(bebasNeueFont, ren) {}

GameStateRenderer::~GameStateRenderer() {
  if (ren) {
    SDL_DestroyRenderer(ren);
  }
}

void GameStateRenderer::render() {
  renderMap();
  renderRoutes();
  renderCities();
  renderPlayerState();
  renderFaceUpTrainCarCards();
  SDL_RenderPresent(ren);
}

void GameStateRenderer::renderMap() {
  SDL_SetRenderDrawColor(ren, 157, 194, 255, 255);
  SDL_RenderClear(ren);
  SDL_RenderCopy(ren, mapTex.get(), nullptr, nullptr);
}

void GameStateRenderer::renderRoutes() {
  std::array<std::pair<int, int>, NumRoutes> from;
  std::array<std::pair<int, int>, NumRoutes> to;
  std::array<std::pair<int, int>, NumRoutes> mid;

  for (auto i = 0uz; i < NumRoutes; i++) {
    auto [xFrom, yFrom] = standardCityCoords[game.map.routes.city1[i]];
    auto [xTo, yTo] = standardCityCoords[game.map.routes.city2[i]];
    const auto &parallelism = game.map.routes.parallelism[i];
    auto xOffs = 0.f;
    auto yOffs = 0.f;
    if (parallelism != Parallelism::None) {
      xOffs = float(yTo - yFrom);
      yOffs = float(xFrom - xTo);
      auto dim = std::sqrtf(xOffs * xOffs + yOffs * yOffs);
      auto dist = parallelism == Parallelism::FirstRoute ? 5 : -5;
      xOffs *= dist / dim;
      yOffs *= dist / dim;
      xFrom += xOffs;
      xTo += xOffs;
      yFrom += yOffs;
      yTo += yOffs;
    }
    from[i] = std::make_pair(xFrom, yFrom);
    to[i] = std::make_pair(xTo, yTo);
    mid[i] = std::make_pair(xFrom + (xTo - xFrom) / 2 + int(yOffs * 3),
                            yFrom + (yTo - yFrom) / 2 - int(xOffs * 3));
  }

  renderRoutesLines(from, to);
  renderRoutesMarkers(mid);
}

void GameStateRenderer::renderRoutesLines(
    const std::array<std::pair<int, int>, NumRoutes> &from,
    const std::array<std::pair<int, int>, NumRoutes> &to) {
  for (auto i = 0uz; i < NumRoutes; i++) {
    auto [xFrom, yFrom] = from[i];
    auto [xTo, yTo] = to[i];
    const auto &claim = game.map.routes.claim[i];
    auto colour = claim == -1 ? SDL_Colour{128, 128, 128, 255}
                              : playerIdxToSdlColour(claim);
    drawLine(ren, xFrom, yFrom, xTo, yTo, 4, colour);
  }
}

void GameStateRenderer::renderRoutesMarkers(
    const std::array<std::pair<int, int>, NumRoutes> &mid) {

  for (auto i = 0uz; i < NumRoutes; i++) {
    auto [xMid, yMid] = mid[i];
    const auto &colour = game.map.routes.colour[i];
    const auto &length = game.map.routes.length[i];
    const auto &numFerries = game.map.routes.numFerries[i];
    const auto &isTunnel = game.map.routes.isTunnel[i];

    auto boxColour = routeColourToSdlColour(colour);
    auto textColour = routeColourToSdlContrastColour(colour);

    if (isTunnel) {
      renderRouteMarkerTunnelBorder(xMid, yMid, textColour);
    }
    renderRouteMarkerBox(xMid, yMid, boxColour);
    condRenderFerryMarkers(numFerries, xMid, yMid, textColour);
    renderRouteMarkerLabelText(xMid, yMid, length, textColour);
  }
}

void GameStateRenderer::renderRouteMarkerTunnelBorder(int x, int y,
                                                      SDL_Colour colour) {
  SDL_SetRenderDrawColor(ren, colour.r, colour.g, colour.b, colour.a);
  auto rect = SDL_Rect{.x = x - 12, .y = y - 12, .w = 24, .h = 24};
  SDL_RenderFillRect(ren, &rect);
}

void GameStateRenderer::renderRouteMarkerBox(int x, int y, SDL_Colour colour) {
  SDL_SetRenderDrawColor(ren, colour.r, colour.g, colour.b, colour.a);
  auto rect = SDL_Rect{.x = x - 10, .y = y - 10, .w = 20, .h = 20};
  SDL_RenderFillRect(ren, &rect);
}

void GameStateRenderer::condRenderFerryMarkers(uint8_t numFerries, int x, int y,
                                               SDL_Colour colour) {
  if (numFerries >= 1) {
    SDL_SetRenderDrawColor(ren, colour.r, colour.g, colour.b, colour.a);
    auto rect = SDL_Rect{.x = x - 10, .y = y + 6, .w = 4, .h = 4};
    SDL_RenderFillRect(ren, &rect);
  }
  if (numFerries == 2) {
    SDL_SetRenderDrawColor(ren, colour.r, colour.g, colour.b, colour.a);
    auto rect = SDL_Rect{.x = x + 6, .y = y + 6, .w = 4, .h = 4};
    SDL_RenderFillRect(ren, &rect);
  }
}

void GameStateRenderer::renderRouteMarkerLabelText(int x, int y, uint8_t text,
                                                   SDL_Colour colour) {
  auto label =
      Texture::text(bebasNeueFont, std::to_string(text).c_str(), colour, ren);
  const auto &[w, h] = label.dim();
  auto dstRect = SDL_Rect{.x = x - w / 2, .y = y - h / 2, .w = w, .h = h};
  SDL_RenderCopy(ren, label.get(), nullptr, &dstRect);
}

void GameStateRenderer::renderCities() {
  for (auto i = 0uz; i < standardCities.size(); i++) {
    const auto &[x, y] = standardCityCoords[i];
    const auto &textTex = cityTextTex[i];

    auto markerDstRect = SDL_Rect{.x = x - 10, .y = y - 10, .w = 21, .h = 21};
    SDL_RenderCopy(ren, cityMarkerTex.get(), nullptr, &markerDstRect);

    auto trainStationClaim = game.map.trainStationClaims[i];
    if (trainStationClaim != -1) {
      auto colour = playerIdxToSdlColour(trainStationClaim);
      SDL_SetRenderDrawColor(ren, colour.r, colour.g, colour.b, colour.a);
      auto playerMarkerDstRect =
          SDL_Rect{.x = x - 6, .y = y - 6, .w = 13, .h = 13};
      SDL_RenderFillRect(ren, &playerMarkerDstRect);
    }

    auto [textWidth, textHeight] = textTex.dim();
    auto textDstRect = SDL_Rect{
        .x = x - textWidth / 2, .y = y + 8, .w = textWidth, .h = textHeight};
    SDL_RenderCopy(ren, textTex.get(), nullptr, &textDstRect);
  }
}

void GameStateRenderer::renderPlayerState() {
  renderPlayerStatePlayerIdx();
  renderPlayerStateInventory();
  renderPlayerStateTrainCarCardsHand();
  renderPlayerStateDestinationTickets();
}

void GameStateRenderer::renderPlayerStatePlayerIdx() {
  auto colour = SDL_Colour{0, 0, 0, 255};
  auto oss = std::ostringstream();
  oss << "Turn: player #" << int(game.playerIdx);
  auto text = Texture::text(bebasNeueFont, oss.str().c_str(), colour, ren);
  const auto &[w, h] = text.dim();
  auto dstRect = SDL_Rect{.x = 4, .y = 4, .w = w, .h = h};
  SDL_RenderCopy(ren, text.get(), nullptr, &dstRect);
}

void GameStateRenderer::renderPlayerStateInventory() {
  auto colour = SDL_Colour{0, 0, 0, 255};
  auto oss = std::ostringstream();
  oss << "#TrainCars: " << int(game.playerStates[game.playerIdx].trainCars)
      << ", #TrainStations: "
      << int(game.playerStates[game.playerIdx].trainStations);
  auto text = Texture::text(bebasNeueFont, oss.str().c_str(), colour, ren);
  const auto &[w, h] = text.dim();
  auto dstRect = SDL_Rect{.x = 4, .y = 24, .w = w, .h = h};
  SDL_RenderCopy(ren, text.get(), nullptr, &dstRect);
}

void GameStateRenderer::renderPlayerStateTrainCarCardsHand() {
  auto xOffs = 0;
  auto hand = game.playerStates[game.playerIdx].trainCarCards;

  auto renderBox = [this, &xOffs](SDL_Colour colour) {
    SDL_SetRenderDrawColor(ren, colour.r, colour.g, colour.b, colour.a);
    auto rect = SDL_Rect{.x = xOffs * 24 + 4, .y = 44, .w = 20, .h = 20};
    SDL_RenderFillRect(ren, &rect);
  };

  auto renderText = [this, &xOffs](SDL_Colour colour, int num) {
    auto oss = std::ostringstream();
    oss << "x" << num;
    auto text = Texture::text(bebasNeueFont, oss.str().c_str(), colour, ren);
    const auto &[w, h] = text.dim();
    auto dstRect =
        SDL_Rect{.x = xOffs * 24 + 14 - w / 2, .y = 54 - h / 2, .w = w, .h = h};
    SDL_RenderCopy(ren, text.get(), nullptr, &dstRect);
  };

  auto drawCards = [&](int num, RouteColour colour) {
    if (num >= 1) {
      renderBox(routeColourToSdlColour(colour));
      renderText(routeColourToSdlContrastColour(colour), num);
      xOffs++;
    }
  };
  drawCards(hand.numPink, RouteColour::Pink);
  drawCards(hand.numWhite, RouteColour::White);
  drawCards(hand.numBlue, RouteColour::Blue);
  drawCards(hand.numYellow, RouteColour::Yellow);
  drawCards(hand.numOrange, RouteColour::Orange);
  drawCards(hand.numBlack, RouteColour::Black);
  drawCards(hand.numRed, RouteColour::Red);
  drawCards(hand.numGreen, RouteColour::Green);
  drawCards(hand.numLocomotive, RouteColour::Grey);
}

void GameStateRenderer::renderPlayerStateDestinationTickets() {
  auto yOffs = 0;
  auto renderText = [this, &yOffs](std::string str) {
    auto text = Texture::text(bebasNeueFont, str.c_str(),
                              SDL_Colour{0, 0, 0, 255}, ren);
    const auto &[w, h] = text.dim();
    auto dstRect = SDL_Rect{.x = 4, .y = 64 + yOffs * 20, .w = w, .h = h};
    SDL_RenderCopy(ren, text.get(), nullptr, &dstRect);
    yOffs++;
  };
  auto renderTicket = [&renderText](const char *city1, const char *city2,
                                    uint8_t pts) {
    auto oss = std::stringstream();
    oss << "  - " << city1 << " -> " << city2 << " (" << int(pts) << ")";
    return renderText(oss.str());
  };
  renderText("Destination tickets:");
  for (const auto &t :
       game.playerStates[game.playerIdx].destinationTicketCards) {
    auto city1 = standardCities[t.city1];
    auto city2 = standardCities[t.city2];
    renderTicket(city1, city2, t.points);
  }
}

void GameStateRenderer::renderFaceUpTrainCarCards() {
  auto xOffs = 0;
  const auto &cards = game.trainCarCardDeck.getFaceUpCards();

  auto renderBox = [this, &xOffs](RouteColour routeColour) {
    auto colour = routeColourToSdlColour(routeColour);
    SDL_SetRenderDrawColor(ren, colour.r, colour.g, colour.b, colour.a);
    auto rect = SDL_Rect{.x = xOffs * 24 + 4, .y = 776, .w = 20, .h = 20};
    SDL_RenderFillRect(ren, &rect);
  };

  for (const auto &c : cards) {
    // clang-format off
    switch (c) {
    case TrainCarCard::Pink:        renderBox(RouteColour::Pink);    break;
    case TrainCarCard::White:       renderBox(RouteColour::White);   break;
    case TrainCarCard::Blue:        renderBox(RouteColour::Blue);    break;
    case TrainCarCard::Yellow:      renderBox(RouteColour::Yellow);  break;
    case TrainCarCard::Orange:      renderBox(RouteColour::Orange);  break;
    case TrainCarCard::Black:       renderBox(RouteColour::Black);   break;
    case TrainCarCard::Red:         renderBox(RouteColour::Red);     break;
    case TrainCarCard::Green:       renderBox(RouteColour::Green);   break;
    case TrainCarCard::Locomotive:  renderBox(RouteColour::Grey);    break;
    default: throw std::logic_error("Unhandled TrainCarCard case.");
    }
    // clang-format on
    xOffs++;
  }
}