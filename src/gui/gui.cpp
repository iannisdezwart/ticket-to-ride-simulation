#include "gui/gui.hpp"
#include "gui/game_state_renderer.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <stop_token>
#include <thread>

SDL_Window *win;
std::unique_ptr<GameStateRenderer> renderer;

void startGui(const Game &game) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "SDL_Init(): " << SDL_GetError() << std::endl;
    throw std::runtime_error("SDL_Init()");
  }
  auto imgInitFlags = IMG_INIT_PNG;
  if ((IMG_Init(imgInitFlags) & imgInitFlags) != imgInitFlags) {
    std::cerr << "IMG_Init(): " << IMG_GetError() << std::endl;
    throw std::runtime_error("IMG_Init()");
  }
  if (TTF_Init() != 0) {
    std::cerr << "TTF_Init(): " << TTF_GetError() << std::endl;
    throw std::runtime_error("TTF_Init()");
  }

  win = SDL_CreateWindow("TicketToRideSimulation", SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED, 1250, 800, SDL_WINDOW_SHOWN);
  if (win == nullptr) {
    std::cerr << "SDL_CreateWindow():" << SDL_GetError() << std::endl;
    SDL_Quit();
    throw std::runtime_error("SDL_CreateWindow()");
  }

  auto ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
  if (ren == nullptr) {
    std::cerr << "SDL_CreateRenderer(): " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(win);
    SDL_Quit();
    throw std::runtime_error("SDL_CreateRenderer()");
  }

  renderer = std::make_unique<GameStateRenderer>(ren, game);
}

bool runGui() {
  auto ev = SDL_Event();
  SDL_PollEvent(&ev);
  renderer->render();

  if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_SPACE) {
    int x, y;
    SDL_GetMouseState(&x, &y);
    std::cout << "Mouse: " << x << ", " << y << std::endl;
    return true;
  }

  return ev.type != SDL_QUIT;
}

void destroyGui() {
  renderer = nullptr;
  if (win != nullptr) {
    SDL_DestroyWindow(win);
  }
  SDL_Quit();
  IMG_Quit();
  TTF_Quit();
}
