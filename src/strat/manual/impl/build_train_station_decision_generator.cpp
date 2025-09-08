#include "strat/manual/impl/build_train_station_decision_generator.hpp"
#include <iostream>

BuildTrainStationDecisionGenerator::BuildTrainStationDecisionGenerator(
    AsyncReader &reader, const Game &gameState)
    : reader(reader), gameState(gameState) {}

BuildTrainStationDecisionGenerator::Ret
BuildTrainStationDecisionGenerator::poll() {
  switch (state) {
  case State::Started:
    return started();
  case State::ReadingCityInput:
    return readingCityInput();
  case State::WaitingForPayment:
    return waitingForPayment();
  }
}

BuildTrainStationDecisionGenerator::Ret
BuildTrainStationDecisionGenerator::started() {
  std::cout << "The following cities still don't have a train station:"
            << std::endl;

  for (auto i = 0uz; i < gameState.map.trainStationClaims.size(); i++) {
    if (gameState.map.trainStationClaims[i] != -1) {
      continue;
    }
    auto cityName = standardCities[i];
    std::cout << "[" << i << "] " << cityName << std::endl;
  }

  std::cout
      << "Choose the index of the city you want to build a train station in:"
      << std::endl;

  state = State::ReadingCityInput;
  return readingCityInput();
}

BuildTrainStationDecisionGenerator::Ret
BuildTrainStationDecisionGenerator::readingCityInput() {
  auto line = reader.readline();
  if (!line.has_value()) {
    return std::nullopt;
  }

  try {
    cityIdx = std::stoul(line.value());
    if (cityIdx >= NumCities) {
      std::cout << "Specified index out of range. Try choosing again:"
                << std::endl;
      return started();
    }
  } catch (const std::out_of_range &ex) {
    std::cout << "Parse error. Try choosing again:" << std::endl;
    return started();
  }

  paymentGenerator = std::make_unique<PaymentAsyncGenerator>(
      reader, "Choose how to pay for train station:");
  state = State::WaitingForPayment;
  return waitingForPayment();
}

BuildTrainStationDecisionGenerator::Ret
BuildTrainStationDecisionGenerator::waitingForPayment() {
  auto res = paymentGenerator->poll();
  if (!res.has_value()) {
    return std::nullopt;
  }

  return BuildTrainStationDecision{
      .cityIdx = cityIdx,
      .payment = res.value(),
  };
}