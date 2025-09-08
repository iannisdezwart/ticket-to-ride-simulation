#pragma once

#include <SDL2/SDL.h>
#include <algorithm>
#include <array>
#include <string>
#include <vector>

enum struct TrainCarCard : char {
  Pink = 'P',
  White = 'W',
  Blue = 'U',
  Yellow = 'Y',
  Orange = 'O',
  Black = 'B',
  Red = 'R',
  Green = 'G',
  Locomotive = 'L',
};

std::string trainCarCardToStr(TrainCarCard card);
TrainCarCard trainCarCardFromChar(char c);
std::vector<TrainCarCard> parsePaymentString(const std::string &str);