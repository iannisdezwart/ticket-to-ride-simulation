#include "algo/path_possible.hpp"
#include "core/map.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

class PathPossibleTest : public Test {
protected:
  uint8_t playerIdx{12};
  uint8_t opponentIdx{34};
  Map map;
};

TEST_F(PathPossibleTest, PossibleEmptyMap) {
  ASSERT_TRUE(
      pathPossible(map, playerIdx, cityIdx("Edinburgh"), cityIdx("Erzurum")));
}

TEST_F(PathPossibleTest, NotPossibleCompletelyBlockedByOpponent) {
  map.routes.claim[map.routeIdx("London", "Amsterdam")] = opponentIdx;
  map.routes.claim[map.routeIdx("London", "Dieppe")] = opponentIdx;
  map.routes.claim[map.routeIdx("London", "Dieppe", 1)] = opponentIdx;
  ASSERT_FALSE(
      pathPossible(map, playerIdx, cityIdx("Edinburgh"), cityIdx("Erzurum")));
}

TEST_F(PathPossibleTest, PossibleCompletelyTakenByUs) {
  map.routes.claim[map.routeIdx("London", "Amsterdam")] = playerIdx;
  map.routes.claim[map.routeIdx("London", "Dieppe")] = playerIdx;
  map.routes.claim[map.routeIdx("London", "Dieppe", 1)] = playerIdx;
  ASSERT_TRUE(
      pathPossible(map, playerIdx, cityIdx("Edinburgh"), cityIdx("Erzurum")));
}

TEST_F(PathPossibleTest, PossibleCrazyCase) {
  map.routes.claim[map.routeIdx("London", "Dieppe")] = opponentIdx;
  map.routes.claim[map.routeIdx("London", "Dieppe", 1)] = opponentIdx;
  map.routes.claim[map.routeIdx("Bruxelles", "Amsterdam")] = opponentIdx;
  map.routes.claim[map.routeIdx("Frankfurt", "Amsterdam")] = opponentIdx;
  map.routes.claim[map.routeIdx("Frankfurt", "Essen")] = opponentIdx;
  map.routes.claim[map.routeIdx("Berlin", "Essen")] = opponentIdx;
  map.routes.claim[map.routeIdx("Riga", "Petrograd")] = opponentIdx;
  map.routes.claim[map.routeIdx("Wilno", "Petrograd")] = opponentIdx;
  map.routes.claim[map.routeIdx("Moskva", "Smolensk")] = opponentIdx;
  map.routes.claim[map.routeIdx("Kyïv", "Kharkov")] = opponentIdx;
  map.routes.claim[map.routeIdx("Rostov", "Sevastopol")] = opponentIdx;
  map.routes.claim[map.routeIdx("Sochi", "Sevastopol")] = opponentIdx;
  map.routes.claim[map.routeIdx("Erzurum", "Sevastopol")] = opponentIdx;
  map.routes.claim[map.routeIdx("Erzurum", "Angora")] = opponentIdx;
  ASSERT_TRUE(
      pathPossible(map, playerIdx, cityIdx("Edinburgh"), cityIdx("Erzurum")));
}