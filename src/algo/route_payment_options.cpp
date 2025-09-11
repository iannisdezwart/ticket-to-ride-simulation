#include "algo/route_payment_options.hpp"
#include "algo/train_car_card_matches_route_colour.hpp"

std::vector<RoutePaymentOption>
routePaymentOptions(const TrainCarCardHand &hand, const Map &map,
                    uint8_t routeIdx) {
  auto options = std::vector<RoutePaymentOption>();

  auto length = map.routes.length[routeIdx];
  auto isTunnel = map.routes.isTunnel[routeIdx];
  auto numFerries = map.routes.numFerries[routeIdx];
  auto colour = map.routes.colour[routeIdx];
  auto numLocomotivesInHand = hand.count(TrainCarCard::Locomotive);

  if (numLocomotivesInHand < numFerries) {
    return options;
  }

  auto evalPaymentOption = [&](TrainCarCard colour) {
    auto numColourInHand = hand.count(colour);
    if (numColourInHand + numLocomotivesInHand < length) {
      return;
    }

    auto n = std::min(numColourInHand, uint8_t(length - numFerries));
    auto l = length - n;
    auto payment = std::vector<TrainCarCard>(length);
    std::fill(payment.begin(), payment.begin() + n, colour);
    std::fill(payment.begin() + n, payment.end(), TrainCarCard::Locomotive);

    auto v = [](auto &&...args) { return std::vector<TrainCarCard>{args...}; };
    auto addOption = [&options, &payment](std::vector<TrainCarCard> tbp) {
      options.push_back(RoutePaymentOption{
          .payment = std::move(payment),
          .tunnelBackupPayment = std::move(tbp),
      });
    };

    if (!isTunnel) {
      addOption(v());
      return;
    }

    constexpr auto Loco = TrainCarCard::Locomotive;
    auto m = numColourInHand - n;
    auto k = numLocomotivesInHand - l;
    if (m >= 3) {
      addOption(v(colour, colour, colour));
      return;
    }
    if (m >= 2 && k >= 1) {
      addOption(v(colour, colour, Loco));
      return;
    }
    if (m >= 1 && k >= 2) {
      addOption(v(colour, Loco, Loco));
      return;
    }
    if (k >= 3) {
      addOption(v(Loco, Loco, Loco));
      return;
    }
    if (m >= 2) {
      addOption(v(colour, colour));
      return;
    }
    if (m >= 1 && k >= 1) {
      addOption(v(colour, Loco));
      return;
    }
    if (k >= 2) {
      addOption(v(Loco, Loco));
      return;
    }
    if (m >= 1) {
      addOption(v(colour));
      return;
    }
    if (k >= 1) {
      addOption(v(Loco));
      return;
    }
  };

  if (trainCarCardMatchesRouteColour(TrainCarCard::Pink, colour)) {
    evalPaymentOption(TrainCarCard::Pink);
  }
  if (trainCarCardMatchesRouteColour(TrainCarCard::White, colour)) {
    evalPaymentOption(TrainCarCard::White);
  }
  if (trainCarCardMatchesRouteColour(TrainCarCard::Blue, colour)) {
    evalPaymentOption(TrainCarCard::Blue);
  }
  if (trainCarCardMatchesRouteColour(TrainCarCard::Yellow, colour)) {
    evalPaymentOption(TrainCarCard::Yellow);
  }
  if (trainCarCardMatchesRouteColour(TrainCarCard::Orange, colour)) {
    evalPaymentOption(TrainCarCard::Orange);
  }
  if (trainCarCardMatchesRouteColour(TrainCarCard::Black, colour)) {
    evalPaymentOption(TrainCarCard::Black);
  }
  if (trainCarCardMatchesRouteColour(TrainCarCard::Red, colour)) {
    evalPaymentOption(TrainCarCard::Red);
  }
  if (trainCarCardMatchesRouteColour(TrainCarCard::Green, colour)) {
    evalPaymentOption(TrainCarCard::Green);
  }

  return options;
}