#pragma once

#include "core/route_colour.hpp"
#include "core/train_car_card.hpp"
#include <bitset>
#include <iostream>
#include <string_view>

constexpr const int NumCities = 47;
constexpr const std::array<const char *, NumCities> standardCities = {
    "Edinburgh", "London",    "Dieppe",     "Brest",          "Paris",
    "Pamplona",  "Madrid",    "Lisboa",     "Cádiz",          "Barcelona",
    "Marseille", "Amsterdam", "Bruxelles",  "Essen",          "Frankfurt",
    "Zürich",    "Roma",      "Venezia",    "München",        "Berlin",
    "København", "Stockholm", "Danzig",     "Wien",           "Zágráb",
    "Brindisi",  "Palermo",   "Sarajevo",   "Budapest",       "Riga",
    "Warszawa",  "Wilno",     "Sofia",      "Athína",         "Bucuresti",
    "Petrograd", "Smolensk",  "Moskva",     "Kyïv",           "Kharkov",
    "Rostov",    "Sochi",     "Sevastopol", "Constantinople", "Smyrna",
    "Angora",    "Erzurum"};
constexpr uint8_t cityIdx(std::string_view city) {
  for (int i = 0; i < NumCities; ++i) {
    if (standardCities[i] == city) {
      return i;
    }
  }
  std::cerr << "City " << city << " does not exist.";
  throw std::runtime_error("CityNotFound");
}

constexpr const std::array<std::pair<uint16_t, uint16_t>, NumCities>
    standardCityCoords = {
        std::make_pair(276, 127),  std::make_pair(311, 239),
        std::make_pair(319, 354),  std::make_pair(218, 367),
        std::make_pair(361, 444),  std::make_pair(251, 556),
        std::make_pair(159, 640),  std::make_pair(50, 662),
        std::make_pair(106, 731),  std::make_pair(258, 670),
        std::make_pair(435, 566),  std::make_pair(433, 253),
        std::make_pair(391, 330),  std::make_pair(500, 270),
        std::make_pair(475, 340),  std::make_pair(470, 463),
        std::make_pair(587, 600),  std::make_pair(560, 496),
        std::make_pair(543, 391),  std::make_pair(611, 277),
        std::make_pair(566, 168),  std::make_pair(635, 82),
        std::make_pair(685, 205),  std::make_pair(654, 404),
        std::make_pair(672, 502),  std::make_pair(688, 631),
        std::make_pair(653, 752),  std::make_pair(785, 563),
        std::make_pair(742, 428),  std::make_pair(756, 100),
        std::make_pair(753, 252),  std::make_pair(884, 214),
        std::make_pair(869, 529),  std::make_pair(855, 678),
        std::make_pair(910, 452),  std::make_pair(959, 36),
        std::make_pair(954, 166),  std::make_pair(1070, 130),
        std::make_pair(964, 261),  std::make_pair(1083, 276),
        std::make_pair(1150, 289), std::make_pair(1203, 396),
        std::make_pair(1067, 416), std::make_pair(1006, 579),
        std::make_pair(991, 689),  std::make_pair(1093, 621),
        std::make_pair(1217, 563),
};

enum struct Parallelism { None, FirstRoute, SecondRoute };

constexpr const int NumRoutes = 101;
struct Route {
  uint8_t city1;
  uint8_t city2;
  RouteColour colour{RouteColour::Grey};
  uint8_t length;
  uint8_t numFerries{0};
  bool isTunnel{false};
  Parallelism parallelism{Parallelism::None};
};
constexpr const std::array<Route, NumRoutes> standardRoutes = {
    Route{
        .city1 = cityIdx("Edinburgh"),
        .city2 = cityIdx("London"),
        .colour = RouteColour::Black,
        .length = 4,
        .parallelism = Parallelism::FirstRoute,
    },
    Route{
        .city1 = cityIdx("Edinburgh"),
        .city2 = cityIdx("London"),
        .colour = RouteColour::Orange,
        .length = 4,
        .parallelism = Parallelism::SecondRoute,
    },
    Route{
        .city1 = cityIdx("London"),
        .city2 = cityIdx("Dieppe"),
        .length = 2,
        .numFerries = 1,
        .parallelism = Parallelism::FirstRoute,
    },
    Route{
        .city1 = cityIdx("London"),
        .city2 = cityIdx("Dieppe"),
        .length = 2,
        .numFerries = 1,
        .parallelism = Parallelism::SecondRoute,
    },
    Route{
        .city1 = cityIdx("London"),
        .city2 = cityIdx("Amsterdam"),
        .length = 2,
        .numFerries = 2,
    },
    Route{
        .city1 = cityIdx("Amsterdam"),
        .city2 = cityIdx("Essen"),
        .colour = RouteColour::Yellow,
        .length = 3,
    },
    Route{
        .city1 = cityIdx("Essen"),
        .city2 = cityIdx("København"),
        .length = 3,
        .numFerries = 1,
        .parallelism = Parallelism::FirstRoute,
    },
    Route{
        .city1 = cityIdx("Essen"),
        .city2 = cityIdx("København"),
        .length = 3,
        .numFerries = 1,
        .parallelism = Parallelism::SecondRoute,
    },
    Route{
        .city1 = cityIdx("København"),
        .city2 = cityIdx("Stockholm"),
        .colour = RouteColour::Yellow,
        .length = 3,
        .parallelism = Parallelism::FirstRoute,
    },
    Route{
        .city1 = cityIdx("København"),
        .city2 = cityIdx("Stockholm"),
        .colour = RouteColour::White,
        .length = 3,
        .parallelism = Parallelism::SecondRoute,
    },
    Route{
        .city1 = cityIdx("Stockholm"),
        .city2 = cityIdx("Petrograd"),
        .length = 8,
        .isTunnel = true,
    },
    Route{
        .city1 = cityIdx("Petrograd"),
        .city2 = cityIdx("Moskva"),
        .colour = RouteColour::White,
        .length = 4,
    },
    Route{
        .city1 = cityIdx("Petrograd"),
        .city2 = cityIdx("Riga"),
        .length = 4,
    },
    Route{
        .city1 = cityIdx("Riga"),
        .city2 = cityIdx("Danzig"),
        .colour = RouteColour::Black,
        .length = 3,
    },
    Route{
        .city1 = cityIdx("Danzig"),
        .city2 = cityIdx("Berlin"),
        .length = 4,
    },
    Route{
        .city1 = cityIdx("Berlin"),
        .city2 = cityIdx("Essen"),
        .colour = RouteColour::Blue,
        .length = 2,
    },
    Route{
        .city1 = cityIdx("Essen"),
        .city2 = cityIdx("Frankfurt"),
        .colour = RouteColour::Green,
        .length = 2,
    },
    Route{
        .city1 = cityIdx("Frankfurt"),
        .city2 = cityIdx("Amsterdam"),
        .colour = RouteColour::White,
        .length = 2,
    },
    Route{
        .city1 = cityIdx("Bruxelles"),
        .city2 = cityIdx("Amsterdam"),
        .colour = RouteColour::Black,
        .length = 1,
    },
    Route{
        .city1 = cityIdx("Bruxelles"),
        .city2 = cityIdx("Dieppe"),
        .colour = RouteColour::Green,
        .length = 2,
    },
    Route{
        .city1 = cityIdx("Brest"),
        .city2 = cityIdx("Dieppe"),
        .colour = RouteColour::Orange,
        .length = 2,
    },
    Route{
        .city1 = cityIdx("Brest"),
        .city2 = cityIdx("Paris"),
        .colour = RouteColour::Black,
        .length = 3,
    },
    Route{
        .city1 = cityIdx("Dieppe"),
        .city2 = cityIdx("Paris"),
        .colour = RouteColour::Pink,
        .length = 1,
    },
    Route{
        .city1 = cityIdx("Bruxelles"),
        .city2 = cityIdx("Paris"),
        .colour = RouteColour::Yellow,
        .length = 2,
        .parallelism = Parallelism::FirstRoute,
    },
    Route{
        .city1 = cityIdx("Bruxelles"),
        .city2 = cityIdx("Paris"),
        .colour = RouteColour::Red,
        .length = 2,
        .parallelism = Parallelism::SecondRoute,
    },
    Route{
        .city1 = cityIdx("Bruxelles"),
        .city2 = cityIdx("Frankfurt"),
        .colour = RouteColour::Blue,
        .length = 2,
    },
    Route{
        .city1 = cityIdx("Berlin"),
        .city2 = cityIdx("Frankfurt"),
        .colour = RouteColour::Black,
        .length = 3,
        .parallelism = Parallelism::FirstRoute,
    },
    Route{
        .city1 = cityIdx("Berlin"),
        .city2 = cityIdx("Frankfurt"),
        .colour = RouteColour::Red,
        .length = 3,
        .parallelism = Parallelism::SecondRoute,
    },
    Route{
        .city1 = cityIdx("Paris"),
        .city2 = cityIdx("Frankfurt"),
        .colour = RouteColour::White,
        .length = 3,
        .parallelism = Parallelism::FirstRoute,
    },
    Route{
        .city1 = cityIdx("Paris"),
        .city2 = cityIdx("Frankfurt"),
        .colour = RouteColour::Orange,
        .length = 3,
        .parallelism = Parallelism::SecondRoute,
    },
    Route{
        .city1 = cityIdx("Warszawa"),
        .city2 = cityIdx("Berlin"),
        .colour = RouteColour::Pink,
        .length = 4,
        .parallelism = Parallelism::FirstRoute,
    },
    Route{
        .city1 = cityIdx("Warszawa"),
        .city2 = cityIdx("Berlin"),
        .colour = RouteColour::Yellow,
        .length = 4,
        .parallelism = Parallelism::SecondRoute,
    },
    Route{
        .city1 = cityIdx("Warszawa"),
        .city2 = cityIdx("Danzig"),
        .length = 2,
    },
    Route{
        .city1 = cityIdx("Warszawa"),
        .city2 = cityIdx("Wilno"),
        .colour = RouteColour::Red,
        .length = 3,
    },
    Route{
        .city1 = cityIdx("Riga"),
        .city2 = cityIdx("Wilno"),
        .colour = RouteColour::Green,
        .length = 4,
    },
    Route{
        .city1 = cityIdx("Petrograd"),
        .city2 = cityIdx("Wilno"),
        .colour = RouteColour::Blue,
        .length = 4,
    },
    Route{
        .city1 = cityIdx("Smolensk"),
        .city2 = cityIdx("Wilno"),
        .colour = RouteColour::Yellow,
        .length = 3,
    },
    Route{
        .city1 = cityIdx("Smolensk"),
        .city2 = cityIdx("Moskva"),
        .colour = RouteColour::Orange,
        .length = 2,
    },
    Route{
        .city1 = cityIdx("Kharkov"),
        .city2 = cityIdx("Moskva"),
        .length = 4,
    },
    Route{
        .city1 = cityIdx("Kyïv"),
        .city2 = cityIdx("Smolensk"),
        .colour = RouteColour::Red,
        .length = 3,
    },
    Route{
        .city1 = cityIdx("Kyïv"),
        .city2 = cityIdx("Wilno"),
        .length = 2,
    },
    Route{
        .city1 = cityIdx("Kyïv"),
        .city2 = cityIdx("Warszawa"),
        .length = 4,
    },
    Route{
        .city1 = cityIdx("Kyïv"),
        .city2 = cityIdx("Kharkov"),
        .length = 4,
    },
    Route{
        .city1 = cityIdx("Rostov"),
        .city2 = cityIdx("Kharkov"),
        .colour = RouteColour::Green,
        .length = 2,
    },
    Route{
        .city1 = cityIdx("Warszawa"),
        .city2 = cityIdx("Wien"),
        .colour = RouteColour::Blue,
        .length = 4,
    },
    Route{
        .city1 = cityIdx("Berlin"),
        .city2 = cityIdx("Wien"),
        .colour = RouteColour::Green,
        .length = 3,
    },
    Route{
        .city1 = cityIdx("München"),
        .city2 = cityIdx("Frankfurt"),
        .colour = RouteColour::Pink,
        .length = 2,
    },
    Route{
        .city1 = cityIdx("Budapest"),
        .city2 = cityIdx("Kyïv"),
        .length = 6,
        .isTunnel = true,
    },
    Route{
        .city1 = cityIdx("Bucuresti"),
        .city2 = cityIdx("Kyïv"),
        .length = 4,
    },
    Route{
        .city1 = cityIdx("Wien"),
        .city2 = cityIdx("München"),
        .colour = RouteColour::Orange,
        .length = 3,
    },
    Route{
        .city1 = cityIdx("Wien"),
        .city2 = cityIdx("Budapest"),
        .colour = RouteColour::Red,
        .length = 1,
        .parallelism = Parallelism::FirstRoute,
    },
    Route{
        .city1 = cityIdx("Wien"),
        .city2 = cityIdx("Budapest"),
        .colour = RouteColour::White,
        .length = 1,
        .parallelism = Parallelism::SecondRoute,
    },
    Route{
        .city1 = cityIdx("Bucuresti"),
        .city2 = cityIdx("Budapest"),
        .length = 4,
        .isTunnel = true,
    },
    Route{
        .city1 = cityIdx("Bucuresti"),
        .city2 = cityIdx("Sevastopol"),
        .colour = RouteColour::White,
        .length = 4,
    },
    Route{
        .city1 = cityIdx("Rostov"),
        .city2 = cityIdx("Sevastopol"),
        .length = 4,
    },
    Route{
        .city1 = cityIdx("Rostov"),
        .city2 = cityIdx("Sochi"),
        .length = 2,
    },
    Route{
        .city1 = cityIdx("Sevastopol"),
        .city2 = cityIdx("Sochi"),
        .length = 2,
        .numFerries = 1,
    },
    Route{
        .city1 = cityIdx("Erzurum"),
        .city2 = cityIdx("Sochi"),
        .colour = RouteColour::Red,
        .length = 3,
        .isTunnel = true,
    },
    Route{
        .city1 = cityIdx("Erzurum"),
        .city2 = cityIdx("Sevastopol"),
        .length = 4,
        .numFerries = 2,
    },
    Route{
        .city1 = cityIdx("Constantinople"),
        .city2 = cityIdx("Sevastopol"),
        .length = 4,
        .numFerries = 2,
    },
    Route{
        .city1 = cityIdx("Erzurum"),
        .city2 = cityIdx("Angora"),
        .colour = RouteColour::Black,
        .length = 3,
    },
    Route{
        .city1 = cityIdx("Constantinople"),
        .city2 = cityIdx("Angora"),
        .length = 2,
        .isTunnel = true,
    },
    Route{
        .city1 = cityIdx("Smyrna"),
        .city2 = cityIdx("Angora"),
        .colour = RouteColour::Orange,
        .length = 3,
        .isTunnel = true,
    },
    Route{
        .city1 = cityIdx("Smyrna"),
        .city2 = cityIdx("Constantinople"),
        .length = 2,
        .isTunnel = true,
    },
    Route{
        .city1 = cityIdx("Bucuresti"),
        .city2 = cityIdx("Constantinople"),
        .colour = RouteColour::Yellow,
        .length = 3,
    },
    Route{
        .city1 = cityIdx("Bucuresti"),
        .city2 = cityIdx("Sofia"),
        .length = 2,
        .isTunnel = true,
    },
    Route{
        .city1 = cityIdx("Constantinople"),
        .city2 = cityIdx("Sofia"),
        .colour = RouteColour::Blue,
        .length = 3,
    },
    Route{
        .city1 = cityIdx("Athína"),
        .city2 = cityIdx("Smyrna"),
        .length = 2,
        .numFerries = 1,
    },
    Route{
        .city1 = cityIdx("Athína"),
        .city2 = cityIdx("Sofia"),
        .colour = RouteColour::Pink,
        .length = 3,
    },
    Route{
        .city1 = cityIdx("Sarajevo"),
        .city2 = cityIdx("Sofia"),
        .length = 2,
        .isTunnel = true,
    },
    Route{
        .city1 = cityIdx("Sarajevo"),
        .city2 = cityIdx("Budapest"),
        .colour = RouteColour::Pink,
        .length = 3,
    },
    Route{
        .city1 = cityIdx("Zágráb"),
        .city2 = cityIdx("Budapest"),
        .colour = RouteColour::Orange,
        .length = 2,
    },
    Route{
        .city1 = cityIdx("Zágráb"),
        .city2 = cityIdx("Sarajevo"),
        .colour = RouteColour::Red,
        .length = 3,
    },
    Route{
        .city1 = cityIdx("Zágráb"),
        .city2 = cityIdx("Wien"),
        .length = 2,
    },
    Route{
        .city1 = cityIdx("Athína"),
        .city2 = cityIdx("Sarajevo"),
        .colour = RouteColour::Green,
        .length = 4,
    },
    Route{
        .city1 = cityIdx("Athína"),
        .city2 = cityIdx("Brindisi"),
        .length = 4,
        .numFerries = 1,
    },
    Route{
        .city1 = cityIdx("Venezia"),
        .city2 = cityIdx("Zágráb"),
        .length = 2,
    },
    Route{
        .city1 = cityIdx("Venezia"),
        .city2 = cityIdx("München"),
        .colour = RouteColour::Blue,
        .length = 2,
        .isTunnel = true,
    },
    Route{
        .city1 = cityIdx("Zürich"),
        .city2 = cityIdx("München"),
        .colour = RouteColour::Yellow,
        .length = 2,
        .isTunnel = true,
    },
    Route{
        .city1 = cityIdx("Zürich"),
        .city2 = cityIdx("Venezia"),
        .colour = RouteColour::Green,
        .length = 2,
        .isTunnel = true,
    },
    Route{
        .city1 = cityIdx("Zürich"),
        .city2 = cityIdx("Paris"),
        .length = 3,
        .isTunnel = true,
    },
    Route{
        .city1 = cityIdx("Zürich"),
        .city2 = cityIdx("Marseille"),
        .colour = RouteColour::Pink,
        .length = 2,
        .isTunnel = true,
    },
    Route{
        .city1 = cityIdx("Venezia"),
        .city2 = cityIdx("Roma"),
        .colour = RouteColour::Black,
        .length = 2,
    },
    Route{
        .city1 = cityIdx("Brindisi"),
        .city2 = cityIdx("Roma"),
        .colour = RouteColour::White,
        .length = 2,
    },
    Route{
        .city1 = cityIdx("Smyrna"),
        .city2 = cityIdx("Palermo"),
        .length = 6,
        .numFerries = 2,
    },
    Route{
        .city1 = cityIdx("Brindisi"),
        .city2 = cityIdx("Palermo"),
        .length = 3,
        .numFerries = 1,
    },
    Route{
        .city1 = cityIdx("Roma"),
        .city2 = cityIdx("Palermo"),
        .length = 4,
        .numFerries = 1,
    },
    Route{
        .city1 = cityIdx("Roma"),
        .city2 = cityIdx("Marseille"),
        .length = 4,
        .isTunnel = true,
    },
    Route{
        .city1 = cityIdx("Paris"),
        .city2 = cityIdx("Marseille"),
        .length = 4,
    },
    Route{
        .city1 = cityIdx("Paris"),
        .city2 = cityIdx("Pamplona"),
        .colour = RouteColour::Blue,
        .length = 4,
        .parallelism = Parallelism::FirstRoute,
    },
    Route{
        .city1 = cityIdx("Paris"),
        .city2 = cityIdx("Pamplona"),
        .colour = RouteColour::Green,
        .length = 4,
        .parallelism = Parallelism::SecondRoute,
    },
    Route{
        .city1 = cityIdx("Brest"),
        .city2 = cityIdx("Pamplona"),
        .colour = RouteColour::Pink,
        .length = 4,
    },
    Route{
        .city1 = cityIdx("Marseille"),
        .city2 = cityIdx("Pamplona"),
        .colour = RouteColour::Red,
        .length = 4,
    },
    Route{
        .city1 = cityIdx("Marseille"),
        .city2 = cityIdx("Barcelona"),
        .length = 4,
    },
    Route{
        .city1 = cityIdx("Pamplona"),
        .city2 = cityIdx("Barcelona"),
        .length = 2,
        .isTunnel = true,
    },
    Route{
        .city1 = cityIdx("Madrid"),
        .city2 = cityIdx("Barcelona"),
        .colour = RouteColour::Yellow,
        .length = 2,
    },
    Route{
        .city1 = cityIdx("Madrid"),
        .city2 = cityIdx("Pamplona"),
        .colour = RouteColour::White,
        .length = 3,
        .isTunnel = true,
        .parallelism = Parallelism::FirstRoute,
    },
    Route{
        .city1 = cityIdx("Madrid"),
        .city2 = cityIdx("Pamplona"),
        .colour = RouteColour::Black,
        .length = 3,
        .isTunnel = true,
        .parallelism = Parallelism::SecondRoute,
    },
    Route{
        .city1 = cityIdx("Madrid"),
        .city2 = cityIdx("Lisboa"),
        .colour = RouteColour::Pink,
        .length = 3,
    },
    Route{
        .city1 = cityIdx("Cádiz"),
        .city2 = cityIdx("Lisboa"),
        .colour = RouteColour::Blue,
        .length = 2,
    },
    Route{
        .city1 = cityIdx("Cádiz"),
        .city2 = cityIdx("Madrid"),
        .colour = RouteColour::Orange,
        .length = 3,
    },
};

struct Routes {
  std::array<uint8_t, NumRoutes> city1;
  std::array<uint8_t, NumRoutes> city2;
  std::array<RouteColour, NumRoutes> colour;
  std::array<uint8_t, NumRoutes> length;
  std::array<uint8_t, NumRoutes> numFerries;
  std::bitset<NumRoutes> isTunnel;
  std::array<Parallelism, NumRoutes> parallelism;
  std::array<int8_t /* PlayerIdx */, NumRoutes> claim;
};

struct Map {
  Routes routes;
  std::array<int8_t /* PlayerIdx */, NumCities> trainStationClaims;

  Map();
  uint8_t routeIdx(const char *city1, const char *city2);
};