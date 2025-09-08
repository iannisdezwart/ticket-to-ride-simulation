#include "strat/manual/impl/payment_async_generator.hpp"
#include <optional>

PaymentAsyncGenerator::PaymentAsyncGenerator(AsyncReader &reader,
                                             std::string_view prompt)
    : reader(reader), prompt(std::move(prompt)) {}

PaymentAsyncGenerator::Ret PaymentAsyncGenerator::poll() {
  // clang-format off
  switch (state) {
  case State::Started:       return started();
  case State::ReadingInput:  return readingInput();
  }
  // clang-format on
}

PaymentAsyncGenerator::Ret PaymentAsyncGenerator::started() {
  std::cout << prompt << std::endl;
  state = State::ReadingInput;
  return readingInput();
}

PaymentAsyncGenerator::Ret PaymentAsyncGenerator::readingInput() {
  auto line = reader.readline();
  if (!line.has_value()) {
    return std::nullopt;
  }

  try {
    return parsePaymentString(line.value());
  } catch (const std::out_of_range &ex) {
    std::cout << "Invalid payment string, try again:" << std::endl;
    return started();
  }
}