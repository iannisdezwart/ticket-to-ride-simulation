#pragma once

#include <iostream>
#include <string>
#include <optional>

class AsyncReader {
private:
  std::string buffer;

public:
  AsyncReader() = default;

  std::optional<std::string> readline();
};