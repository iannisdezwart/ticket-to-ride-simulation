#include "algo/train_car_card_matches_route_colour.hpp"

bool trainCarCardMatchesRouteColour(TrainCarCard t, RouteColour r) {
  if (r == RouteColour::Grey) {
    return true;
  }
  // clang-format off
  switch (t) {
  case TrainCarCard::Pink:        return r == RouteColour::Pink;
  case TrainCarCard::White:       return r == RouteColour::White;
  case TrainCarCard::Blue:        return r == RouteColour::Blue;
  case TrainCarCard::Yellow:      return r == RouteColour::Yellow;
  case TrainCarCard::Orange:      return r == RouteColour::Orange;
  case TrainCarCard::Black:       return r == RouteColour::Black;
  case TrainCarCard::Red:         return r == RouteColour::Red;
  case TrainCarCard::Green:       return r == RouteColour::Green;
  case TrainCarCard::Locomotive:  return true;
  default: throw std::out_of_range("Invalid TrainCarCard enumeration value.");
  }
  // clang-format on
}