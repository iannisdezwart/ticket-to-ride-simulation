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

namespace detail {
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
} // namespace detail

template <typename T, typename U, typename F = detail::Identity>
AsyncGeneratorPtr<T> transform(std::unique_ptr<U> gen, F f = {}) {
  return std::make_unique<detail::TransformedAsyncGenerator<T, U, F>>(
      std::move(gen), std::move(f));
}

namespace detail {
template <typename T>
class PlainValueAsyncGenerator : public AsyncGenerator<T> {
private:
  T val;

public:
  PlainValueAsyncGenerator(T val) : val(std::move(val)) {}
  AsyncGenerator<T>::Ret poll() override { return std::make_optional(val); }
};
} // namespace detail

template <typename T> AsyncGeneratorPtr<T> plainValue(T val) {
  return std::make_unique<detail::PlainValueAsyncGenerator<T>>(std::move(val));
}