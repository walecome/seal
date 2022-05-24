#include "CrashHandler.hh"

#include <fmt/format.h>

namespace {
  static CrashHandler* static_instance = nullptr;
}

CrashHandler* CrashHandler::the() {
  if (static_instance == nullptr) {
    static_instance = new CrashHandler();
  }
  return static_instance;
}

CrashHandler::CrashHandler() = default;

void CrashHandler::verify_not_reached(std::string_view message) {
  fmt::print("{}\n", message);
  exit(1);
}

void CrashHandler::verify(bool condition, std::string_view message) {
  if (condition) {
    return;
  }
  fmt::print("{}\n", message);
  exit(1);
}
