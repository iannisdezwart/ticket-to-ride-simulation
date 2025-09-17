#pragma once

#include "core/route_colour.hpp"
#include "core/train_car_card.hpp"
#include <bitset>
#include <iostream>
#include <string_view>

constexpr const int NumCities = 47;
constexpr const std::array<const char *, NumCities> standardCities = {
    "Edinburgh", "London",    "Dieppe",     "Brest",          "Paris",     // 4
    "Pamplona",  "Madrid",    "Lisboa",     "Cádiz",          "Barcelona", // 9
    "Marseille", "Amsterdam", "Bruxelles",  "Essen",          "Frankfurt", // 14
    "Zürich",    "Roma",      "Venezia",    "München",        "Berlin",    // 19
    "København", "Stockholm", "Danzig",     "Wien",           "Zágráb",    // 24
    "Brindisi",  "Palermo",   "Sarajevo",   "Budapest",       "Riga",      // 29
    "Warszawa",  "Wilno",     "Sofia",      "Athína",         "Bucuresti", // 34
    "Petrograd", "Smolensk",  "Moskva",     "Kyïv",           "Kharkov",   // 39
    "Rostov",    "Sochi",     "Sevastopol", "Constantinople", "Smyrna",    // 44
    "Angora",    "Erzurum"};                                               // 46
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
    }, // 0
    Route{
        .city1 = cityIdx("Edinburgh"),
        .city2 = cityIdx("London"),
        .colour = RouteColour::Orange,
        .length = 4,
        .parallelism = Parallelism::SecondRoute,
    }, // 1
    Route{
        .city1 = cityIdx("London"),
        .city2 = cityIdx("Dieppe"),
        .length = 2,
        .numFerries = 1,
        .parallelism = Parallelism::FirstRoute,
    }, // 2
    Route{
        .city1 = cityIdx("London"),
        .city2 = cityIdx("Dieppe"),
        .length = 2,
        .numFerries = 1,
        .parallelism = Parallelism::SecondRoute,
    }, // 3
    Route{
        .city1 = cityIdx("London"),
        .city2 = cityIdx("Amsterdam"),
        .length = 2,
        .numFerries = 2,
    }, // 4
    Route{
        .city1 = cityIdx("Amsterdam"),
        .city2 = cityIdx("Essen"),
        .colour = RouteColour::Yellow,
        .length = 3,
    }, // 5
    Route{
        .city1 = cityIdx("Essen"),
        .city2 = cityIdx("København"),
        .length = 3,
        .numFerries = 1,
        .parallelism = Parallelism::FirstRoute,
    }, // 6
    Route{
        .city1 = cityIdx("Essen"),
        .city2 = cityIdx("København"),
        .length = 3,
        .numFerries = 1,
        .parallelism = Parallelism::SecondRoute,
    }, // 7
    Route{
        .city1 = cityIdx("København"),
        .city2 = cityIdx("Stockholm"),
        .colour = RouteColour::Yellow,
        .length = 3,
        .parallelism = Parallelism::FirstRoute,
    }, // 8
    Route{
        .city1 = cityIdx("København"),
        .city2 = cityIdx("Stockholm"),
        .colour = RouteColour::White,
        .length = 3,
        .parallelism = Parallelism::SecondRoute,
    }, // 9
    Route{
        .city1 = cityIdx("Stockholm"),
        .city2 = cityIdx("Petrograd"),
        .length = 8,
        .isTunnel = true,
    }, // 10
    Route{
        .city1 = cityIdx("Petrograd"),
        .city2 = cityIdx("Moskva"),
        .colour = RouteColour::White,
        .length = 4,
    }, // 11
    Route{
        .city1 = cityIdx("Riga"),
        .city2 = cityIdx("Petrograd"),
        .length = 4,
    }, // 12
    Route{
        .city1 = cityIdx("Danzig"),
        .city2 = cityIdx("Riga"),
        .colour = RouteColour::Black,
        .length = 3,
    }, // 13
    Route{
        .city1 = cityIdx("Berlin"),
        .city2 = cityIdx("Danzig"),
        .length = 4,
    }, // 14
    Route{
        .city1 = cityIdx("Essen"),
        .city2 = cityIdx("Berlin"),
        .colour = RouteColour::Blue,
        .length = 2,
    }, // 15
    Route{
        .city1 = cityIdx("Essen"),
        .city2 = cityIdx("Frankfurt"),
        .colour = RouteColour::Green,
        .length = 2,
    }, // 16
    Route{
        .city1 = cityIdx("Amsterdam"),
        .city2 = cityIdx("Frankfurt"),
        .colour = RouteColour::White,
        .length = 2,
    }, // 17
    Route{
        .city1 = cityIdx("Amsterdam"),
        .city2 = cityIdx("Bruxelles"),
        .colour = RouteColour::Black,
        .length = 1,
    }, // 18
    Route{
        .city1 = cityIdx("Dieppe"),
        .city2 = cityIdx("Bruxelles"),
        .colour = RouteColour::Green,
        .length = 2,
    }, // 19
    Route{
        .city1 = cityIdx("Dieppe"),
        .city2 = cityIdx("Brest"),
        .colour = RouteColour::Orange,
        .length = 2,
    }, // 20
    Route{
        .city1 = cityIdx("Brest"),
        .city2 = cityIdx("Paris"),
        .colour = RouteColour::Black,
        .length = 3,
    }, // 21
    Route{
        .city1 = cityIdx("Dieppe"),
        .city2 = cityIdx("Paris"),
        .colour = RouteColour::Pink,
        .length = 1,
    }, // 22
    Route{
        .city1 = cityIdx("Paris"),
        .city2 = cityIdx("Bruxelles"),
        .colour = RouteColour::Yellow,
        .length = 2,
        .parallelism = Parallelism::FirstRoute,
    }, // 23
    Route{
        .city1 = cityIdx("Paris"),
        .city2 = cityIdx("Bruxelles"),
        .colour = RouteColour::Red,
        .length = 2,
        .parallelism = Parallelism::SecondRoute,
    }, // 24
    Route{
        .city1 = cityIdx("Bruxelles"),
        .city2 = cityIdx("Frankfurt"),
        .colour = RouteColour::Blue,
        .length = 2,
    }, // 25
    Route{
        .city1 = cityIdx("Frankfurt"),
        .city2 = cityIdx("Berlin"),
        .colour = RouteColour::Black,
        .length = 3,
        .parallelism = Parallelism::FirstRoute,
    }, // 26
    Route{
        .city1 = cityIdx("Frankfurt"),
        .city2 = cityIdx("Berlin"),
        .colour = RouteColour::Red,
        .length = 3,
        .parallelism = Parallelism::SecondRoute,
    }, // 27
    Route{
        .city1 = cityIdx("Paris"),
        .city2 = cityIdx("Frankfurt"),
        .colour = RouteColour::White,
        .length = 3,
        .parallelism = Parallelism::FirstRoute,
    }, // 28
    Route{
        .city1 = cityIdx("Paris"),
        .city2 = cityIdx("Frankfurt"),
        .colour = RouteColour::Orange,
        .length = 3,
        .parallelism = Parallelism::SecondRoute,
    }, // 29
    Route{
        .city1 = cityIdx("Berlin"),
        .city2 = cityIdx("Warszawa"),
        .colour = RouteColour::Pink,
        .length = 4,
        .parallelism = Parallelism::FirstRoute,
    }, // 30
    Route{
        .city1 = cityIdx("Berlin"),
        .city2 = cityIdx("Warszawa"),
        .colour = RouteColour::Yellow,
        .length = 4,
        .parallelism = Parallelism::SecondRoute,
    }, // 31
    Route{
        .city1 = cityIdx("Danzig"),
        .city2 = cityIdx("Warszawa"),
        .length = 2,
    }, // 32
    Route{
        .city1 = cityIdx("Warszawa"),
        .city2 = cityIdx("Wilno"),
        .colour = RouteColour::Red,
        .length = 3,
    }, // 33
    Route{
        .city1 = cityIdx("Riga"),
        .city2 = cityIdx("Wilno"),
        .colour = RouteColour::Green,
        .length = 4,
    }, // 34
    Route{
        .city1 = cityIdx("Wilno"),
        .city2 = cityIdx("Petrograd"),
        .colour = RouteColour::Blue,
        .length = 4,
    }, // 35
    Route{
        .city1 = cityIdx("Wilno"),
        .city2 = cityIdx("Smolensk"),
        .colour = RouteColour::Yellow,
        .length = 3,
    }, // 36
    Route{
        .city1 = cityIdx("Smolensk"),
        .city2 = cityIdx("Moskva"),
        .colour = RouteColour::Orange,
        .length = 2,
    }, // 37
    Route{
        .city1 = cityIdx("Moskva"),
        .city2 = cityIdx("Kharkov"),
        .length = 4,
    }, // 38
    Route{
        .city1 = cityIdx("Smolensk"),
        .city2 = cityIdx("Kyïv"),
        .colour = RouteColour::Red,
        .length = 3,
    }, // 39
    Route{
        .city1 = cityIdx("Wilno"),
        .city2 = cityIdx("Kyïv"),
        .length = 2,
    }, // 40
    Route{
        .city1 = cityIdx("Warszawa"),
        .city2 = cityIdx("Kyïv"),
        .length = 4,
    }, // 41
    Route{
        .city1 = cityIdx("Kyïv"),
        .city2 = cityIdx("Kharkov"),
        .length = 4,
    }, // 42
    Route{
        .city1 = cityIdx("Kharkov"),
        .city2 = cityIdx("Rostov"),
        .colour = RouteColour::Green,
        .length = 2,
    }, // 43
    Route{
        .city1 = cityIdx("Wien"),
        .city2 = cityIdx("Warszawa"),
        .colour = RouteColour::Blue,
        .length = 4,
    }, // 44
    Route{
        .city1 = cityIdx("Berlin"),
        .city2 = cityIdx("Wien"),
        .colour = RouteColour::Green,
        .length = 3,
    }, // 45
    Route{
        .city1 = cityIdx("Frankfurt"),
        .city2 = cityIdx("München"),
        .colour = RouteColour::Pink,
        .length = 2,
    }, // 46
    Route{
        .city1 = cityIdx("Budapest"),
        .city2 = cityIdx("Kyïv"),
        .length = 6,
        .isTunnel = true,
    }, // 47
    Route{
        .city1 = cityIdx("Bucuresti"),
        .city2 = cityIdx("Kyïv"),
        .length = 4,
    }, // 48
    Route{
        .city1 = cityIdx("München"),
        .city2 = cityIdx("Wien"),
        .colour = RouteColour::Orange,
        .length = 3,
    }, // 49
    Route{
        .city1 = cityIdx("Wien"),
        .city2 = cityIdx("Budapest"),
        .colour = RouteColour::Red,
        .length = 1,
        .parallelism = Parallelism::FirstRoute,
    }, // 50
    Route{
        .city1 = cityIdx("Wien"),
        .city2 = cityIdx("Budapest"),
        .colour = RouteColour::White,
        .length = 1,
        .parallelism = Parallelism::SecondRoute,
    }, // 51
    Route{
        .city1 = cityIdx("Budapest"),
        .city2 = cityIdx("Bucuresti"),
        .length = 4,
        .isTunnel = true,
    }, // 52
    Route{
        .city1 = cityIdx("Bucuresti"),
        .city2 = cityIdx("Sevastopol"),
        .colour = RouteColour::White,
        .length = 4,
    }, // 53
    Route{
        .city1 = cityIdx("Rostov"),
        .city2 = cityIdx("Sevastopol"),
        .length = 4,
    }, // 54
    Route{
        .city1 = cityIdx("Rostov"),
        .city2 = cityIdx("Sochi"),
        .length = 2,
    }, // 55
    Route{
        .city1 = cityIdx("Sochi"),
        .city2 = cityIdx("Sevastopol"),
        .length = 2,
        .numFerries = 1,
    }, // 56
    Route{
        .city1 = cityIdx("Sochi"),
        .city2 = cityIdx("Erzurum"),
        .colour = RouteColour::Red,
        .length = 3,
        .isTunnel = true,
    }, // 57
    Route{
        .city1 = cityIdx("Sevastopol"),
        .city2 = cityIdx("Erzurum"),
        .length = 4,
        .numFerries = 2,
    }, // 58
    Route{
        .city1 = cityIdx("Sevastopol"),
        .city2 = cityIdx("Constantinople"),
        .length = 4,
        .numFerries = 2,
    }, // 59
    Route{
        .city1 = cityIdx("Angora"),
        .city2 = cityIdx("Erzurum"),
        .colour = RouteColour::Black,
        .length = 3,
    }, // 60
    Route{
        .city1 = cityIdx("Constantinople"),
        .city2 = cityIdx("Angora"),
        .length = 2,
        .isTunnel = true,
    }, // 61
    Route{
        .city1 = cityIdx("Smyrna"),
        .city2 = cityIdx("Angora"),
        .colour = RouteColour::Orange,
        .length = 3,
        .isTunnel = true,
    }, // 62
    Route{
        .city1 = cityIdx("Constantinople"),
        .city2 = cityIdx("Smyrna"),
        .length = 2,
        .isTunnel = true,
    }, // 63
    Route{
        .city1 = cityIdx("Bucuresti"),
        .city2 = cityIdx("Constantinople"),
        .colour = RouteColour::Yellow,
        .length = 3,
    }, // 64
    Route{
        .city1 = cityIdx("Sofia"),
        .city2 = cityIdx("Bucuresti"),
        .length = 2,
        .isTunnel = true,
    }, // 65
    Route{
        .city1 = cityIdx("Sofia"),
        .city2 = cityIdx("Constantinople"),
        .colour = RouteColour::Blue,
        .length = 3,
    }, // 66
    Route{
        .city1 = cityIdx("Athína"),
        .city2 = cityIdx("Smyrna"),
        .length = 2,
        .numFerries = 1,
    }, // 67
    Route{
        .city1 = cityIdx("Sofia"),
        .city2 = cityIdx("Athína"),
        .colour = RouteColour::Pink,
        .length = 3,
    }, // 68
    Route{
        .city1 = cityIdx("Sarajevo"),
        .city2 = cityIdx("Sofia"),
        .length = 2,
        .isTunnel = true,
    }, // 69
    Route{
        .city1 = cityIdx("Sarajevo"),
        .city2 = cityIdx("Budapest"),
        .colour = RouteColour::Pink,
        .length = 3,
    }, // 70
    Route{
        .city1 = cityIdx("Zágráb"),
        .city2 = cityIdx("Budapest"),
        .colour = RouteColour::Orange,
        .length = 2,
    }, // 71
    Route{
        .city1 = cityIdx("Zágráb"),
        .city2 = cityIdx("Sarajevo"),
        .colour = RouteColour::Red,
        .length = 3,
    }, // 72
    Route{
        .city1 = cityIdx("Wien"),
        .city2 = cityIdx("Zágráb"),
        .length = 2,
    }, // 73
    Route{
        .city1 = cityIdx("Sarajevo"),
        .city2 = cityIdx("Athína"),
        .colour = RouteColour::Green,
        .length = 4,
    }, // 74
    Route{
        .city1 = cityIdx("Brindisi"),
        .city2 = cityIdx("Athína"),
        .length = 4,
        .numFerries = 1,
    }, // 75
    Route{
        .city1 = cityIdx("Venezia"),
        .city2 = cityIdx("Zágráb"),
        .length = 2,
    }, // 76
    Route{
        .city1 = cityIdx("Venezia"),
        .city2 = cityIdx("München"),
        .colour = RouteColour::Blue,
        .length = 2,
        .isTunnel = true,
    }, // 77
    Route{
        .city1 = cityIdx("Zürich"),
        .city2 = cityIdx("München"),
        .colour = RouteColour::Yellow,
        .length = 2,
        .isTunnel = true,
    }, // 78
    Route{
        .city1 = cityIdx("Zürich"),
        .city2 = cityIdx("Venezia"),
        .colour = RouteColour::Green,
        .length = 2,
        .isTunnel = true,
    }, // 79
    Route{
        .city1 = cityIdx("Paris"),
        .city2 = cityIdx("Zürich"),
        .length = 3,
        .isTunnel = true,
    }, // 80
    Route{
        .city1 = cityIdx("Marseille"),
        .city2 = cityIdx("Zürich"),
        .colour = RouteColour::Pink,
        .length = 2,
        .isTunnel = true,
    }, // 81
    Route{
        .city1 = cityIdx("Roma"),
        .city2 = cityIdx("Venezia"),
        .colour = RouteColour::Black,
        .length = 2,
    }, // 82
    Route{
        .city1 = cityIdx("Roma"),
        .city2 = cityIdx("Brindisi"),
        .colour = RouteColour::White,
        .length = 2,
    }, // 83
    Route{
        .city1 = cityIdx("Palermo"),
        .city2 = cityIdx("Smyrna"),
        .length = 6,
        .numFerries = 2,
    }, // 84
    Route{
        .city1 = cityIdx("Brindisi"),
        .city2 = cityIdx("Palermo"),
        .length = 3,
        .numFerries = 1,
    }, // 85
    Route{
        .city1 = cityIdx("Roma"),
        .city2 = cityIdx("Palermo"),
        .length = 4,
        .numFerries = 1,
    }, // 86
    Route{
        .city1 = cityIdx("Marseille"),
        .city2 = cityIdx("Roma"),
        .length = 4,
        .isTunnel = true,
    }, // 87
    Route{
        .city1 = cityIdx("Paris"),
        .city2 = cityIdx("Marseille"),
        .length = 4,
    }, // 88
    Route{
        .city1 = cityIdx("Paris"),
        .city2 = cityIdx("Pamplona"),
        .colour = RouteColour::Blue,
        .length = 4,
        .parallelism = Parallelism::FirstRoute,
    }, // 89
    Route{
        .city1 = cityIdx("Paris"),
        .city2 = cityIdx("Pamplona"),
        .colour = RouteColour::Green,
        .length = 4,
        .parallelism = Parallelism::SecondRoute,
    }, // 90
    Route{
        .city1 = cityIdx("Brest"),
        .city2 = cityIdx("Pamplona"),
        .colour = RouteColour::Pink,
        .length = 4,
    }, // 91
    Route{
        .city1 = cityIdx("Pamplona"),
        .city2 = cityIdx("Marseille"),
        .colour = RouteColour::Red,
        .length = 4,
    }, // 92
    Route{
        .city1 = cityIdx("Barcelona"),
        .city2 = cityIdx("Marseille"),
        .length = 4,
    }, // 93
    Route{
        .city1 = cityIdx("Pamplona"),
        .city2 = cityIdx("Barcelona"),
        .length = 2,
        .isTunnel = true,
    }, // 94
    Route{
        .city1 = cityIdx("Madrid"),
        .city2 = cityIdx("Barcelona"),
        .colour = RouteColour::Yellow,
        .length = 2,
    }, // 95
    Route{
        .city1 = cityIdx("Pamplona"),
        .city2 = cityIdx("Madrid"),
        .colour = RouteColour::White,
        .length = 3,
        .isTunnel = true,
        .parallelism = Parallelism::FirstRoute,
    }, // 96
    Route{
        .city1 = cityIdx("Pamplona"),
        .city2 = cityIdx("Madrid"),
        .colour = RouteColour::Black,
        .length = 3,
        .isTunnel = true,
        .parallelism = Parallelism::SecondRoute,
    }, // 97
    Route{
        .city1 = cityIdx("Madrid"),
        .city2 = cityIdx("Lisboa"),
        .colour = RouteColour::Pink,
        .length = 3,
    }, // 98
    Route{
        .city1 = cityIdx("Lisboa"),
        .city2 = cityIdx("Cádiz"),
        .colour = RouteColour::Blue,
        .length = 2,
    }, // 99
    Route{
        .city1 = cityIdx("Madrid"),
        .city2 = cityIdx("Cádiz"),
        .colour = RouteColour::Orange,
        .length = 3,
    }, // 100
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
  uint8_t routeIdx(const char *city1, const char *city2, int n = 0);
};