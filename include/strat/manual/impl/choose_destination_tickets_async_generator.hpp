#pragma once

#include "core/destination_ticket_card.hpp"
#include "util/async_reader.hpp"
#include <array>
#include <cstdint>
#include <sstream>
#include <string_view>

template <size_t N>
class ChooseDestinationTicketsAsyncGenerator : public AsyncGenerator<uint8_t> {
public:
  ChooseDestinationTicketsAsyncGenerator(
      AsyncReader &reader, std::array<DestinationTicketCard, N> cards,
      std::string_view propmt)
      : reader(reader), cards(std::move(cards)), prompt(std::move(propmt)) {}

  std::optional<uint8_t> poll() override {
    // clang-format off
    switch (state) {
    case State::Started:       return started();
    case State::ReadingInput:  return readingInput();
    default:                   throw std::logic_error("Unreachable.");
    }
    // clang-format on
  }

private:
  AsyncReader &reader;
  std::array<DestinationTicketCard, N> cards;
  uint8_t value;
  std::string_view prompt;
  enum struct State {
    Started,
    ReadingInput,
  } state;

  std::optional<uint8_t> started() {
    std::cout << prompt << std::endl;
    for (auto i = 0uz; i < cards.size(); i++) {
      const auto &route = cards[i];
      std::cout << "[" << i << "] " << standardCities[route.city1] << " -> "
                << standardCities[route.city2] << " (" << int(route.points)
                << ") pts." << std::endl;
    }

    return choose();
  }

  std::optional<uint8_t> choose() {
    std::cout
        << "Choose the indexes of the routes to keep, separated by spaces:"
        << std::endl;

    state = State::ReadingInput;
    return readingInput();
  }

  std::optional<uint8_t> readingInput() {
    auto line = reader.readline();
    if (!line.has_value()) {
      return std::nullopt;
    }

    std::istringstream iss(line.value());
    std::string segment;
    auto bitMask = uint8_t(0);
    while (std::getline(iss, segment, ' ')) {
      try {
        auto idx = std::stoul(segment);
        if (idx >= cards.size()) {
          std::cout << "Specified index out of range. Try choosing again:"
                    << std::endl;
          return choose();
        }
        bitMask |= (1 << idx);
      } catch (const std::out_of_range &ex) {
        std::cout << "Parse error. Try choosing again:" << std::endl;
        return choose();
      }
    }

    return std::make_optional(bitMask);
  }
};