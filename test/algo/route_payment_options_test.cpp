#include "algo/route_payment_options.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

class RoutePaymentOptionsTest : public Test {
protected:
  Map map;

  static constexpr auto L = TrainCarCard::Locomotive;
  static constexpr auto B = TrainCarCard::Blue;
  static constexpr auto R = TrainCarCard::Red;

  TrainCarCardHand makeHand(auto &&...cards) {
    auto h = TrainCarCardHand();
    (h.add(cards), ...);
    return h;
  }
};

TEST_F(RoutePaymentOptionsTest, ColourOnlyPass) {
  auto h = makeHand(B, B, B, R, R);
  auto routeIdx = map.routeIdx("Sofia", "Constantinople");
  auto opts = routePaymentOptions(h, map, routeIdx);
  ASSERT_THAT(opts, ElementsAre(Field("payment", &RoutePaymentOption::payment,
                                      ElementsAre(B, B, B))));
}

TEST_F(RoutePaymentOptionsTest, ColourOnlyFail) {
  auto h = makeHand(R, R, B, B);
  auto routeIdx = map.routeIdx("Sofia", "Constantinople");
  auto opts = routePaymentOptions(h, map, routeIdx);
  ASSERT_THAT(opts, IsEmpty());
}

TEST_F(RoutePaymentOptionsTest, LocomotiveSavesTheDayPass) {
  auto h = makeHand(B, B, L);
  auto routeIdx = map.routeIdx("Sofia", "Constantinople");
  auto opts = routePaymentOptions(h, map, routeIdx);
  ASSERT_THAT(opts, ElementsAre(Field("payment", &RoutePaymentOption::payment,
                                      ElementsAre(B, B, L))));
}

TEST_F(RoutePaymentOptionsTest, LocomotivesOnlyPass) {
  auto h = makeHand(L, L, L);
  auto routeIdx = map.routeIdx("Sofia", "Constantinople");
  auto opts = routePaymentOptions(h, map, routeIdx);
  ASSERT_THAT(opts, ElementsAre(Field("payment", &RoutePaymentOption::payment,
                                      ElementsAre(L, L, L))));
}

TEST_F(RoutePaymentOptionsTest, TunnelColourOnlyPass) {
  auto h = makeHand(R, R, R, R, B, B);
  auto routeIdx = map.routeIdx("Sochi", "Erzurum");
  auto opts = routePaymentOptions(h, map, routeIdx);
  ASSERT_THAT(
      opts,
      ElementsAre(AllOf(
          Field("payment", &RoutePaymentOption::payment, ElementsAre(R, R, R)),
          Field("tunnelBackupPayment", &RoutePaymentOption::tunnelBackupPayment,
                ElementsAre(R)))));
}

TEST_F(RoutePaymentOptionsTest, TunnelLocomotivesInBackupPass) {
  auto h = makeHand(R, R, R, R, L, L, L, B, B);
  auto routeIdx = map.routeIdx("Sochi", "Erzurum");
  auto opts = routePaymentOptions(h, map, routeIdx);
  ASSERT_THAT(
      opts,
      ElementsAre(AllOf(
          Field("payment", &RoutePaymentOption::payment, ElementsAre(R, R, R)),
          Field("tunnelBackupPayment", &RoutePaymentOption::tunnelBackupPayment,
                ElementsAre(R, L, L)))));
}

TEST_F(RoutePaymentOptionsTest, GreyRouteMultipleOptions) {
  auto h = makeHand(R, R, R, R, B, B, B, L, L);
  auto routeIdx = map.routeIdx("Riga", "Petrograd");
  auto opts = routePaymentOptions(h, map, routeIdx);
  ASSERT_THAT(opts, ElementsAre(Field("payment", &RoutePaymentOption::payment,
                                      ElementsAre(B, B, B, L)),
                                Field("payment", &RoutePaymentOption::payment,
                                      ElementsAre(R, R, R, R))));
}

TEST_F(RoutePaymentOptionsTest, FerryRouteFailsWithNotEnoughLocomotives) {
  auto h = makeHand(R, R, R, R, R, R, R, R, L);
  auto routeIdx = map.routeIdx("Palermo", "Smyrna");
  auto opts = routePaymentOptions(h, map, routeIdx);
  ASSERT_THAT(opts, IsEmpty());
}

TEST_F(RoutePaymentOptionsTest, FerryRouteIsPaidWithEnoughLocomotives) {
  auto h = makeHand(R, R, R, R, R, R, R, R, L, L, L);
  auto routeIdx = map.routeIdx("Palermo", "Smyrna");
  auto opts = routePaymentOptions(h, map, routeIdx);
  ASSERT_THAT(opts, ElementsAre(Field("payment", &RoutePaymentOption::payment,
                                      ElementsAre(R, R, R, R, L, L))));
}

TEST_F(RoutePaymentOptionsTest, FerryRouteIsPaidWithEnoughLocomotives2) {
  auto h = makeHand(R, R, R, L, L);
  auto routeIdx = map.routeIdx("Sevastopol", "Erzurum");
  auto opts = routePaymentOptions(h, map, routeIdx);
  ASSERT_THAT(opts, ElementsAre(Field("payment", &RoutePaymentOption::payment,
                                      ElementsAre(R, R, L, L))));
}