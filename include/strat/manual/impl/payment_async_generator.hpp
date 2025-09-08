#pragma once

#include "core/train_car_card.hpp"
#include "util/async_generator.hpp"
#include "util/async_reader.hpp"
#include <vector>

class PaymentAsyncGenerator : public AsyncGenerator<std::vector<TrainCarCard>> {
public:
  PaymentAsyncGenerator(AsyncReader &reader, std::string_view prompt);
  Ret poll() override;

private:
  AsyncReader &reader;
  std::string_view prompt;
  enum struct State {
    Started,
    ReadingInput,
  } state;

  Ret started();
  Ret readingInput();
};