#include "algo/routes_strategy/routes_strategy.hpp"

struct SerialDijkstraRoutesStrategy : public RoutesStrategy {
  ~SerialDijkstraRoutesStrategy() noexcept = default;

  std::optional<TurnDecision> operator()(const Game &gameState) override;
};