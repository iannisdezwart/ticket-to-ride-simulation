#include "algo/routes_strategy/routes_strategy.hpp"

class SteinerForestRoutesStrategy : public RoutesStrategy {
public:
  ~SteinerForestRoutesStrategy() noexcept = default;
  std::optional<TurnDecision> operator()(const Game &gameState) override;

private:
  std::vector<uint8_t> cache;
  int8_t numDestinationTicketCards{-1};

  bool cacheInvalid(uint8_t cardsSize, const Game &gameState);
};