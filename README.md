# TicketToRideSimulation

Simulating Ticket To Ride: Europe to find the best strategies.
Work in progress...

## Dependencies

SDL2, GTest & OR-Tools.

- `brew install sdl2 sdl_image sdl2_ttf sdl2_gfx`
- `brew install gtest`
- `brew install ortools`

## Build & run

- `mkdir -p build`
- `cmake -S . -B build`
- `cmake --build build`
- Run `build/tests` to run all tests.
- Run `build/simulate <strategy_name_1> ... <strategy_name_n>` to run a simulation.

## Core game

Please refer to the [official game manual](http://www.gamingcorner.nl/rules/boardgames/ticket%20to%20ride%20europe_uk.pdf).
I've tried to use the exact naming of items and mechanics from the official manual.

Everything in the game manual is essentially outlined and implemented in the `include/core/` and `src/core/` directories.
The main `Game` class can be found in `include/core/game.hpp`. It holds the entire game state:
- The deck of train car cards (`include/core/train_car_card_deck.hpp`).
- The deck of destination ticket cards (`include/core/destination_ticket_card_deck.hpp`)
- All players' states (`include/core/player_state.hpp`).
- The state of the map, which consists of claimed routes and claimed train stations (`include/core/map.hpp`).
- Whose turn it is.
Additionally, the players' strategies are also saved in the `Game` class.

`Game::run()` will step through the game. It is non-blocking, so that the GUI can also run and I/O can be handled.
This is especially important for `include/core/strat/manual/manual_player_strategy.hpp`, since it has to wait for the player to make a decision.
Other automated strategies may have to perform a lot of computation and can therefore decide to yield to allow for I/O processing.
In each step, pending processing is awaited, and `takeTurn()` is called on the current player's strategy.
After the strategy has finished processing and made a decision, the `Game` class will update the game state based on the player's decision.

## Strategies

The interface `include/core/strat/player_strategy.hpp` can be implemented to create a strategy for simulation.
There are just two member functions to implement: `chooseInitialDestinationTickets()` and `takeTurn()`.
The decisions that can be made are listed in `include/turn/turn_decision.hpp`.

### Manual strategy

Propmts what to do to `std::cout` and reads decisions from `std::cin`.
Useful for debugging, or just to play the game in a very cumbersome fashion.

### Greedy strategy

This is a super simple strategy that just keeps all 4 destination tickets at the start of the game.
It will then focus on the routes one by one, claiming all routes that are on the shortest path.
When the strategy doesn't have enough train car cards to build any route, it will take two cards from the face-down draw pile.
Rinse and repeat.

### More strategies...

Work in progress 🛠️.

## GUI

This project offers a GUI that is useful for `include/strat/manual/manual_player_strategy.hpp` and debugging.
It uses SDL2 under the hood and is very basic. The map, all public game state and the current player's private information are displayed.
<img width="1253" height="829" alt="Screenshot 2025-09-08 at 14 12 09" src="https://github.com/user-attachments/assets/2738cd7d-4df6-4a98-8bcc-237f47c8bd46" />
