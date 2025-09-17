#include "algo/routes_strategy/serial_dijkstra_routes_strategy.hpp"
#include "algo/routes_strategy/steiner_forest_routes_strategy.hpp"
#include "core/game.hpp"
#include "eval/completed_destination_tickets.hpp"
#include "gui/gui.hpp"
#include "strat/greedy/greedy_player_strategy.hpp"
#include "strat/long_routes/long_routes_player_strategy.hpp"
#include "strat/manual/manual_player_strategy.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <thread>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0]
              << " <strategy_name_1> ... <strategy_name_n>" << std::endl;
    return 1;
  }

  auto numPlayers = argc - 1;
  if (numPlayers > 5) {
    std::cerr << "Invalid number of player strategies given." << std::endl
              << "Maximum number of players is 5." << std::endl;
    return 1;
  }

  auto playerStrategies =
      std::vector<std::unique_ptr<PlayerStrategy>>(numPlayers);

  for (auto i = 0; i < numPlayers; i++) {
    const auto &arg = argv[1 + i];
    auto &s = playerStrategies[i];
    if (std::strcmp(arg, "manual") == 0) {
      s = std::make_unique<ManualPlayerStrategy>();
    } else if (std::strcmp(arg, "greedy") == 0) {
      s = std::make_unique<GreedyPlayerStrategy>(
          std::make_unique<SerialDijkstraRoutesStrategy>());
    } else if (std::strcmp(arg, "greedy-steiner") == 0) {
      s = std::make_unique<GreedyPlayerStrategy>(
          std::make_unique<SteinerForestRoutesStrategy>());
    } else if (std::strcmp(arg, "long-routes") == 0) {
      s = std::make_unique<LongRoutesPlayerStrategy>(
          std::make_unique<SteinerForestRoutesStrategy>());
    } else {
      std::cerr << "Invalid strategy name '" << arg << "'." << std::endl
                << "Valid strategies: 'manual', 'greedy', 'greedy-steiner'."
                << std::endl;
      return 1;
    }
  }

  auto game = Game(std::move(playerStrategies));
  startGui(game);

  // auto gameInterval = std::chrono::milliseconds(500);
  auto gameInterval = std::chrono::milliseconds(0);
  auto lastGameRunTime = std::chrono::high_resolution_clock::time_point();
  while (true) {
    auto now = std::chrono::high_resolution_clock::now();
    if (now - lastGameRunTime > gameInterval) {
      lastGameRunTime = now;
      if (!game.run()) {
        break;
      }
    }
    if (!runGui()) {
      break;
    }
    std::this_thread::yield();
  }

  std::cout << "Game has finished. Results:" << std::endl;
  auto reports = game.makePlayerReports();
  for (const auto &rep : reports) {
    std::cout << "Player #" << int(rep.playerIdx) << std::endl;
    std::cout << " - totalPoints: " << rep.totalPoints << std::endl;
    std::cout << " - completedRoutesPoints: " << rep.completedRoutesPoints
              << std::endl;
    std::cout << " - completedDestinationTicketsPoints: "
              << rep.completedDestinationTickets.points << " (";
    for (const auto &x : rep.completedDestinationTickets.completed) {
      std::cout << int(x) << ", ";
    }
    std::cout << ")" << std::endl;
    std::cout << " - unplayedTrainStationPoints: "
              << rep.unplayedTrainStationPoints << std::endl;
    std::cout << " - europeanExpressBonusPoints: "
              << rep.europeanExpressBonusPoints << std::endl;
  }
  std::cout << "Player #" << int(reports.front().playerIdx) << " won the game!"
            << std::endl;

  std::cout << "Game has finished. Press any key to exit..." << std::endl;
  std::cin.get();

  destroyGui();
  return 0;
}