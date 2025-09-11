#include "eval/completed_routes.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

class CompletedRoutesTest : public Test {
protected:
  Map map;
  int8_t playerIdx1{0};
  int8_t playerIdx2{1};
};

TEST_F(CompletedRoutesTest, SimpleTest) {
  map.routes.claim[map.routeIdx("London", "Amsterdam")] = playerIdx1;
  map.routes.claim[map.routeIdx("Amsterdam", "Essen")] = playerIdx1;
  map.routes.claim[map.routeIdx("Palermo", "Smyrna")] = playerIdx1;
  map.routes.claim[map.routeIdx("Roma", "Brindisi")] = playerIdx2;
  auto rep = evalCompletedRoutes(2, map.routes);
  ASSERT_THAT(rep, ElementsAre(21, 2));
}
