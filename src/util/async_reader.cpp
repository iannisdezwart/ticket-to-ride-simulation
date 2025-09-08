#include "util/async_reader.hpp"
#include <iostream>
#include <sys/select.h>
#include <unistd.h>

std::optional<std::string> AsyncReader::readline() {
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);
  timeval tv{0, 0}; // non-blocking

  int ret = select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv);
  if (ret > 0 && FD_ISSET(STDIN_FILENO, &fds)) {
    char buf[256];
    ssize_t n = read(STDIN_FILENO, buf, sizeof(buf));
    if (n > 0) {
      buffer.append(buf, n);
    }
  }

  size_t pos = buffer.find('\n');
  if (pos != std::string::npos) {
    std::string line = buffer.substr(0, pos);
    buffer.erase(0, pos + 1);
    return line;
  }

  return std::nullopt;
}