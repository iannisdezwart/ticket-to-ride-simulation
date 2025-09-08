#include "core/game.hpp"
#include "gui/gui.hpp"
#include "strat/manual/manual_strategy.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <thread>

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <num_players>" << std::endl;
    return 1;
  }

  auto numPlayers = int();
  try {
    numPlayers = std::atol(argv[1]);
    if (numPlayers == 0 || numPlayers > 5) {
      std::cerr << "Invalid number of players given." << std::endl;
      return 1;
    }
  } catch (const std::out_of_range &ex) {
    std::cerr << "Invalid number of players given." << std::endl;
    return 1;
  }

  auto playerStrategies =
      std::vector<std::unique_ptr<PlayerStrategy>>(numPlayers);
  std::generate(playerStrategies.begin(), playerStrategies.end(),
                []() { return std::make_unique<ManualStrategy>(); });

  auto game = Game(std::move(playerStrategies));
  startGui(game);

  while (true) {
    runGui();
    game.run();
    std::this_thread::yield();
  }

  destroyGui();
  return 0;
}