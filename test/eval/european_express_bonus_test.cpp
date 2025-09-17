#include "eval/european_express_bonus.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

class EuropeanExpressBonusTest : public Test {
protected:
  Map map;
  int8_t playerIdx1{0};
  int8_t playerIdx2{1};
};

TEST_F(EuropeanExpressBonusTest, SimpleTest) {
  map.routes.claim[map.routeIdx("London", "Amsterdam")] = playerIdx1;
  map.routes.claim[map.routeIdx("Amsterdam", "Essen")] = playerIdx1;
  map.routes.claim[map.routeIdx("Essen", "Berlin")] = playerIdx1;
  map.routes.claim[map.routeIdx("Palermo", "Smyrna")] = playerIdx1;
  auto longest = computeLongestRoute(playerIdx1, map.routes);
  ASSERT_THAT(longest, Eq(7));
  auto bonus = evalEuropeanExpressBonus(1, map.routes);
  ASSERT_THAT(bonus, ElementsAre(10));
}

TEST_F(EuropeanExpressBonusTest, OnePlayerWins) {
  map.routes.claim[map.routeIdx("London", "Amsterdam")] = playerIdx1;
  map.routes.claim[map.routeIdx("Amsterdam", "Essen")] = playerIdx1;
  map.routes.claim[map.routeIdx("Essen", "Berlin")] = playerIdx1;
  map.routes.claim[map.routeIdx("Palermo", "Smyrna")] = playerIdx2;
  auto longest1 = computeLongestRoute(playerIdx1, map.routes);
  ASSERT_THAT(longest1, Eq(7));
  auto longest2 = computeLongestRoute(playerIdx2, map.routes);
  ASSERT_THAT(longest2, Eq(6));
  auto bonus = evalEuropeanExpressBonus(2, map.routes);
  ASSERT_THAT(bonus, ElementsAre(10, 0));
}

TEST_F(EuropeanExpressBonusTest, BothPlayersWin) {
  map.routes.claim[map.routeIdx("London", "Amsterdam")] = playerIdx1;
  map.routes.claim[map.routeIdx("Amsterdam", "Frankfurt")] = playerIdx1;
  map.routes.claim[map.routeIdx("Frankfurt", "Essen")] = playerIdx1;
  map.routes.claim[map.routeIdx("Palermo", "Smyrna")] = playerIdx2;
  auto longest1 = computeLongestRoute(playerIdx1, map.routes);
  ASSERT_THAT(longest1, Eq(6));
  auto longest2 = computeLongestRoute(playerIdx2, map.routes);
  ASSERT_THAT(longest2, Eq(6));
  auto bonus = evalEuropeanExpressBonus(2, map.routes);
  ASSERT_THAT(bonus, ElementsAre(10, 10));
}

TEST_F(EuropeanExpressBonusTest, Cycle) {
  map.routes.claim[map.routeIdx("Paris", "Zürich")] = playerIdx1;
  map.routes.claim[map.routeIdx("Zürich", "München")] = playerIdx1;
  map.routes.claim[map.routeIdx("München", "Venezia")] = playerIdx1;
  map.routes.claim[map.routeIdx("Venezia", "Zürich")] = playerIdx1;
  map.routes.claim[map.routeIdx("Venezia", "Roma")] = playerIdx1;
  auto longest = computeLongestRoute(playerIdx1, map.routes);
  ASSERT_THAT(longest, Eq(9));
  auto bonus = evalEuropeanExpressBonus(1, map.routes);
  ASSERT_THAT(bonus, ElementsAre(10));
}

TEST_F(EuropeanExpressBonusTest, Branch) {
  map.routes.claim[map.routeIdx("Budapest", "Bucuresti")] = playerIdx1;
  map.routes.claim[map.routeIdx("Bucuresti", "Sevastopol")] = playerIdx1;
  map.routes.claim[map.routeIdx("Bucuresti", "Sofia")] = playerIdx1;
  auto longest = computeLongestRoute(playerIdx1, map.routes);
  ASSERT_THAT(longest, Eq(8));
  auto bonus = evalEuropeanExpressBonus(1, map.routes);
  ASSERT_THAT(bonus, ElementsAre(10));
}
