#include "algo/shortest_path.hpp"
#include "core/map.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

class ShortestPathTest : public Test {
protected:
  uint8_t playerIdx{12};
  uint8_t opponentIdx{34};
  Map map;
};

TEST_F(ShortestPathTest, EmptyMap) {
  auto p = shortestPath(map, playerIdx, cityIdx("London"), cityIdx("Berlin"));
  ASSERT_THAT(p, ElementsAre(map.routeIdx("Frankfurt", "Berlin"),
                             map.routeIdx("Amsterdam", "Frankfurt"),
                             map.routeIdx("London", "Amsterdam")));
}

TEST_F(ShortestPathTest, PartOfRouteAlreadyPlacedOneSide) {
  map.routes.claim[map.routeIdx("London", "Amsterdam")] = playerIdx;
  auto p = shortestPath(map, playerIdx, cityIdx("London"), cityIdx("Berlin"));
  ASSERT_THAT(p, ElementsAre(map.routeIdx("Frankfurt", "Berlin"),
                             map.routeIdx("Amsterdam", "Frankfurt")));
}

TEST_F(ShortestPathTest, PartOfRouteAlreadyPlacedOneBothSides) {
  map.routes.claim[map.routeIdx("London", "Amsterdam")] = playerIdx;
  map.routes.claim[map.routeIdx("Frankfurt", "Berlin")] = playerIdx;
  auto p = shortestPath(map, playerIdx, cityIdx("London"), cityIdx("Berlin"));
  ASSERT_THAT(p, ElementsAre(map.routeIdx("Amsterdam", "Frankfurt")));
}

TEST_F(ShortestPathTest, PartOfRouteAlreadyPlacedInTheMiddle) {
  map.routes.claim[map.routeIdx("Amsterdam", "Frankfurt")] = playerIdx;
  auto p = shortestPath(map, playerIdx, cityIdx("London"), cityIdx("Berlin"));
  ASSERT_THAT(p, ElementsAre(map.routeIdx("Frankfurt", "Berlin"),
                             map.routeIdx("London", "Amsterdam")));
}

TEST_F(ShortestPathTest, OtherPlayerInTheWay1) {
  map.routes.claim[map.routeIdx("London", "Amsterdam")] = opponentIdx;
  auto p = shortestPath(map, playerIdx, cityIdx("London"), cityIdx("Berlin"));
  ASSERT_THAT(p, ElementsAre(map.routeIdx("Frankfurt", "Berlin"),
                             map.routeIdx("Paris", "Frankfurt"),
                             map.routeIdx("Dieppe", "Paris"),
                             map.routeIdx("London", "Dieppe")));
}

TEST_F(ShortestPathTest, OtherPlayerInTheWay2) {
  map.routes.claim[map.routeIdx("Frankfurt", "Amsterdam")] = opponentIdx;
  auto p = shortestPath(map, playerIdx, cityIdx("London"), cityIdx("Berlin"));
  ASSERT_THAT(p, ElementsAre(map.routeIdx("Essen", "Berlin"),
                             map.routeIdx("Amsterdam", "Essen"),
                             map.routeIdx("London", "Amsterdam")));
}

TEST_F(ShortestPathTest, RouteImpossible) {
  map.routes.claim[map.routeIdx("Cádiz", "Madrid")] = opponentIdx;
  map.routes.claim[map.routeIdx("Cádiz", "Lisboa")] = opponentIdx;
  auto p = shortestPath(map, playerIdx, cityIdx("Barcelona"), cityIdx("Cádiz"));
  ASSERT_THAT(p, IsEmpty());
}