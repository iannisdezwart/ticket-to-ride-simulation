#include "core/map.hpp"
#include "eval/completed_destination_tickets.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <sstream>

using namespace testing;

class CompletedDestinationTicketsTest : public Test {
protected:
  Map map;
  int8_t playerIdx{54};
  int8_t opponentIdx{87};
};

TEST_F(CompletedDestinationTicketsTest, NoCompletedRoutes) {
  auto cards = std::vector<DestinationTicketCard>{
      DestinationTicketCard{
          .city1 = cityIdx("London"),
          .city2 = cityIdx("Amsterdam"),
          .points = 99,
      },
  };
  auto rep = evalCompletedDestinationTickets(playerIdx, map, cards);
  ASSERT_THAT(rep.completed, ElementsAre(false));
  ASSERT_THAT(rep.points, Eq(0));
}

TEST_F(CompletedDestinationTicketsTest, OneSimpleCompletedRoute) {
  map.routes.claim[map.routeIdx("London", "Amsterdam")] = playerIdx;
  auto cards = std::vector<DestinationTicketCard>{
      DestinationTicketCard{
          .city1 = cityIdx("London"),
          .city2 = cityIdx("Amsterdam"),
          .points = 99,
      },
  };
  auto rep = evalCompletedDestinationTickets(playerIdx, map, cards);
  ASSERT_THAT(rep.completed, ElementsAre(true));
  ASSERT_THAT(rep.points, Eq(99));
}

TEST_F(CompletedDestinationTicketsTest, OneComplexCompletedRoute) {
  map.routes.claim[map.routeIdx("London", "Amsterdam")] = playerIdx;
  map.routes.claim[map.routeIdx("Amsterdam", "Essen")] = playerIdx;
  auto cards = std::vector<DestinationTicketCard>{
      DestinationTicketCard{
          .city1 = cityIdx("London"),
          .city2 = cityIdx("Essen"),
          .points = 99,
      },
  };
  auto rep = evalCompletedDestinationTickets(playerIdx, map, cards);
  ASSERT_THAT(rep.completed, ElementsAre(true));
  ASSERT_THAT(rep.points, Eq(99));
}

TEST_F(CompletedDestinationTicketsTest, TwoSeparateComplexCompletedRoutes) {
  map.routes.claim[map.routeIdx("London", "Amsterdam")] = playerIdx;
  map.routes.claim[map.routeIdx("Essen", "Berlin")] = playerIdx;
  auto cards = std::vector<DestinationTicketCard>{
      DestinationTicketCard{
          .city1 = cityIdx("London"),
          .city2 = cityIdx("Amsterdam"),
          .points = 99,
      },
      DestinationTicketCard{
          .city1 = cityIdx("Essen"),
          .city2 = cityIdx("Berlin"),
          .points = 98,
      },
  };
  auto rep = evalCompletedDestinationTickets(playerIdx, map, cards);
  ASSERT_THAT(rep.completed, ElementsAre(true, true));
  ASSERT_THAT(rep.points, Eq(197));
}

TEST_F(CompletedDestinationTicketsTest,
       ShortRouteFullyEnclosedByLongRouteBothCompleted) {
  map.routes.claim[map.routeIdx("London", "Amsterdam")] = playerIdx;
  map.routes.claim[map.routeIdx("Essen", "Amsterdam")] = playerIdx;
  map.routes.claim[map.routeIdx("Essen", "Berlin")] = playerIdx;
  auto cards = std::vector<DestinationTicketCard>{
      DestinationTicketCard{
          .city1 = cityIdx("London"),
          .city2 = cityIdx("Berlin"),
          .points = 99,
      },
      DestinationTicketCard{
          .city1 = cityIdx("Essen"),
          .city2 = cityIdx("Amsterdam"),
          .points = 98,
      },
  };
  auto rep = evalCompletedDestinationTickets(playerIdx, map, cards);
  ASSERT_THAT(rep.completed, ElementsAre(true, true));
  ASSERT_THAT(rep.points, Eq(197));
}

TEST_F(CompletedDestinationTicketsTest, ComplexRouteAlmostCompleted) {
  map.routes.claim[map.routeIdx("London", "Amsterdam")] = playerIdx;
  map.routes.claim[map.routeIdx("Essen", "Amsterdam")] = playerIdx;
  auto cards = std::vector<DestinationTicketCard>{
      DestinationTicketCard{
          .city1 = cityIdx("London"),
          .city2 = cityIdx("Berlin"),
          .points = 99,
      },
  };
  auto rep = evalCompletedDestinationTickets(playerIdx, map, cards);
  ASSERT_THAT(rep.completed, ElementsAre(false));
  ASSERT_THAT(rep.points, Eq(0));
}

TEST_F(CompletedDestinationTicketsTest, ComplexRouteSavedByTrainStation) {
  map.routes.claim[map.routeIdx("London", "Amsterdam")] = playerIdx;
  map.routes.claim[map.routeIdx("Essen", "Amsterdam")] = playerIdx;
  map.routes.claim[map.routeIdx("Essen", "Berlin")] = opponentIdx;
  map.trainStationClaims[cityIdx("Berlin")] = playerIdx;
  auto cards = std::vector<DestinationTicketCard>{
      DestinationTicketCard{
          .city1 = cityIdx("London"),
          .city2 = cityIdx("Berlin"),
          .points = 99,
      },
  };
  auto rep = evalCompletedDestinationTickets(playerIdx, map, cards);
  ASSERT_THAT(rep.completed, ElementsAre(true));
  ASSERT_THAT(rep.points, Eq(99));
}

TEST_F(CompletedDestinationTicketsTest,
       ComplexRouteNotSavedByTrainStationBecauseOpponentDidntBuild) {
  map.routes.claim[map.routeIdx("London", "Amsterdam")] = playerIdx;
  map.routes.claim[map.routeIdx("Essen", "Amsterdam")] = playerIdx;
  map.trainStationClaims[cityIdx("Berlin")] = playerIdx;
  auto cards = std::vector<DestinationTicketCard>{
      DestinationTicketCard{
          .city1 = cityIdx("London"),
          .city2 = cityIdx("Berlin"),
          .points = 99,
      },
  };
  auto rep = evalCompletedDestinationTickets(playerIdx, map, cards);
  ASSERT_THAT(rep.completed, ElementsAre(false));
  ASSERT_THAT(rep.points, Eq(0));
}

TEST_F(CompletedDestinationTicketsTest, TrainStationTakesRouteWithMostPoints) {
  map.routes.claim[map.routeIdx("Zágráb", "Wien")] = playerIdx;
  map.routes.claim[map.routeIdx("Wien", "Berlin")] = opponentIdx;
  map.routes.claim[map.routeIdx("Wien", "Warszawa")] = opponentIdx;
  map.trainStationClaims[cityIdx("Wien")] = playerIdx;
  auto cards = std::vector<DestinationTicketCard>{
      DestinationTicketCard{
          .city1 = cityIdx("Zágráb"),
          .city2 = cityIdx("Berlin"),
          .points = 55,
      },
      DestinationTicketCard{
          .city1 = cityIdx("Zágráb"),
          .city2 = cityIdx("Warszawa"),
          .points = 66,
      },
  };
  auto rep = evalCompletedDestinationTickets(playerIdx, map, cards);
  ASSERT_THAT(rep.completed, ElementsAre(false, true));
  ASSERT_THAT(rep.points, Eq(11));
}

TEST_F(CompletedDestinationTicketsTest,
       TwoTrainStationsTakeTwoRoutesNextToEachOther) {
  map.routes.claim[map.routeIdx("Zágráb", "Wien")] = playerIdx;
  map.routes.claim[map.routeIdx("Budapest", "Wien")] = playerIdx;
  map.routes.claim[map.routeIdx("Wien", "Berlin")] = opponentIdx;
  map.routes.claim[map.routeIdx("Wien", "Warszawa")] = opponentIdx;
  map.trainStationClaims[cityIdx("Wien")] = playerIdx;
  map.trainStationClaims[cityIdx("Warszawa")] = playerIdx;
  auto cards = std::vector<DestinationTicketCard>{
      DestinationTicketCard{
          .city1 = cityIdx("Zágráb"),
          .city2 = cityIdx("Berlin"),
          .points = 55,
      },
      DestinationTicketCard{
          .city1 = cityIdx("Budapest"),
          .city2 = cityIdx("Warszawa"),
          .points = 66,
      },
  };
  auto rep = evalCompletedDestinationTickets(playerIdx, map, cards);
  ASSERT_THAT(rep.completed, ElementsAre(true, true));
  ASSERT_THAT(rep.points, Eq(121));
}