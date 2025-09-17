#include "algo/steiner_forest.hpp"
#include "core/map.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

class SteinerForestTest : public Test {
protected:
  uint8_t playerIdx{12};
  uint8_t opponentIdx{34};
  Map map;
};

TEST_F(SteinerForestTest, SingleEmptyMap) {
  auto paths = std::vector<std::pair<uint8_t, uint8_t>>{
      {cityIdx("London"), cityIdx("Berlin")}};
  auto p = steinerForest(map, playerIdx, paths);
  ASSERT_THAT(p, ElementsAre(map.routeIdx("London", "Amsterdam"),
                             map.routeIdx("Amsterdam", "Essen"),
                             map.routeIdx("Essen", "Berlin")));
}

TEST_F(SteinerForestTest, SingleCrazyCase) {
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
  auto paths = std::vector<std::pair<uint8_t, uint8_t>>{
      {cityIdx("Edinburgh"), cityIdx("Erzurum")}};
  auto p = steinerForest(map, playerIdx, paths);
  ASSERT_THAT(p, ElementsAre(map.routeIdx("Edinburgh", "London"),
                             map.routeIdx("London", "Amsterdam"),
                             map.routeIdx("Amsterdam", "Essen"),
                             map.routeIdx("Essen", "København"),
                             map.routeIdx("København", "Stockholm"),
                             map.routeIdx("Stockholm", "Petrograd"),
                             map.routeIdx("Petrograd", "Moskva"),
                             map.routeIdx("Moskva", "Kharkov"),
                             map.routeIdx("Kharkov", "Rostov"),
                             map.routeIdx("Rostov", "Sochi"),
                             map.routeIdx("Sochi", "Erzurum")));
}

TEST_F(SteinerForestTest, TwoRoutesEmptyMap) {
  auto paths = std::vector<std::pair<uint8_t, uint8_t>>{
      {cityIdx("London"), cityIdx("Berlin")},
      {cityIdx("Paris"), cityIdx("Danzig")}};
  auto p = steinerForest(map, playerIdx, paths);
  ASSERT_THAT(p, ElementsAre(map.routeIdx("London", "Dieppe"),
                             map.routeIdx("Berlin", "Danzig"),
                             map.routeIdx("Dieppe", "Paris"),
                             map.routeIdx("Frankfurt", "Berlin"),
                             map.routeIdx("Paris", "Frankfurt")));
}

TEST_F(SteinerForestTest, TwoRoutesPartOfRouteAlreadyExists) {
  map.routes.claim[map.routeIdx("London", "Amsterdam")] = playerIdx;
  auto paths = std::vector<std::pair<uint8_t, uint8_t>>{
      {cityIdx("London"), cityIdx("Berlin")},
      {cityIdx("Paris"), cityIdx("Danzig")}};
  auto p = steinerForest(map, playerIdx, paths);
  ASSERT_THAT(p, ElementsAre(map.routeIdx("London", "Dieppe"),
                             map.routeIdx("Berlin", "Danzig"),
                             map.routeIdx("Amsterdam", "Frankfurt"),
                             map.routeIdx("Dieppe", "Paris"),
                             map.routeIdx("Frankfurt", "Berlin")));
}

TEST_F(SteinerForestTest, TwoRoutesOtherPlayerInTheWay) {
  map.routes.claim[map.routeIdx("Berlin", "Danzig")] = opponentIdx;
  map.routes.claim[map.routeIdx("Amsterdam", "Frankfurt")] = opponentIdx;
  auto paths = std::vector<std::pair<uint8_t, uint8_t>>{
      {cityIdx("London"), cityIdx("Berlin")},
      {cityIdx("Paris"), cityIdx("Danzig")}};
  auto p = steinerForest(map, playerIdx, paths);
  ASSERT_THAT(p, ElementsAre(map.routeIdx("London", "Dieppe"),
                             map.routeIdx("Dieppe", "Paris"),
                             map.routeIdx("Frankfurt", "Berlin"),
                             map.routeIdx("Paris", "Frankfurt"),
                             map.routeIdx("Berlin", "Warszawa"),
                             map.routeIdx("Warszawa", "Danzig")));
}

TEST_F(SteinerForestTest, RouteImpossible) {
  map.routes.claim[map.routeIdx("Cádiz", "Madrid")] = opponentIdx;
  map.routes.claim[map.routeIdx("Cádiz", "Lisboa")] = opponentIdx;
  auto paths = std::vector<std::pair<uint8_t, uint8_t>>{
      {cityIdx("London"), cityIdx("Berlin")},
      {cityIdx("Barcelona"), cityIdx("Cádiz")}};
  auto p = steinerForest(map, playerIdx, paths);
  ASSERT_THAT(p, IsEmpty());
}

TEST_F(SteinerForestTest, FourRoutesEmptyMap) {
  auto paths = std::vector<std::pair<uint8_t, uint8_t>>{
      {cityIdx("Lisboa"), cityIdx("Petrograd")},
      {cityIdx("Edinburgh"), cityIdx("Erzurum")},
      {cityIdx("Stockholm"), cityIdx("Palermo")},
      {cityIdx("Brest"), cityIdx("Rostov")}};
  auto p = steinerForest(map, playerIdx, paths);
  ASSERT_THAT(
      p,
      ElementsAre(
          map.routeIdx("Edinburgh", "London"), map.routeIdx("London", "Dieppe"),
          map.routeIdx("Essen", "København", 1),
          map.routeIdx("København", "Stockholm", 1),
          map.routeIdx("Stockholm", "Petrograd"),
          map.routeIdx("Petrograd", "Moskva"),
          map.routeIdx("Essen", "Frankfurt"), map.routeIdx("Dieppe", "Brest"),
          map.routeIdx("Dieppe", "Paris"), map.routeIdx("Paris", "Frankfurt"),
          map.routeIdx("Moskva", "Kharkov"), map.routeIdx("Kharkov", "Rostov"),
          map.routeIdx("Frankfurt", "München"), map.routeIdx("Rostov", "Sochi"),
          map.routeIdx("Sochi", "Erzurum"), map.routeIdx("Venezia", "München"),
          map.routeIdx("Roma", "Venezia"), map.routeIdx("Roma", "Palermo"),
          map.routeIdx("Paris", "Pamplona"),
          map.routeIdx("Pamplona", "Madrid", 1),
          map.routeIdx("Madrid", "Lisboa")));
}