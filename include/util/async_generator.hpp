#pragma once

#include <memory>
#include <optional>

template <typename T> struct AsyncGenerator {
  virtual ~AsyncGenerator() = default;

  using Ret = std::optional<T>;
  // Performs work and optionally returns the value when finished.
  virtual Ret poll() = 0;
};

template <typename T>
using AsyncGeneratorPtr = std::unique_ptr<AsyncGenerator<T>>;

template <typename T, typename U, typename F>
class TransformedAsyncGenerator : public AsyncGenerator<T> {
private:
  std::unique_ptr<U> gen;
  F f;

public:
  TransformedAsyncGenerator(std::unique_ptr<U> gen, F f)
      : gen(std::move(gen)), f(std::move(f)) {}
  AsyncGenerator<T>::Ret poll() override { return f(gen->poll()); }
};

struct Identity {
  template <typename X> constexpr X operator()(X &&x) const noexcept {
    return std::forward<X>(x);
  }
};

template <typename T, typename U, typename F = Identity>
AsyncGeneratorPtr<T> transformAsyncGenerator(std::unique_ptr<U> gen, F f = {}) {
  return std::make_unique<TransformedAsyncGenerator<T, U, F>>(std::move(gen),
                                                              std::move(f));
}