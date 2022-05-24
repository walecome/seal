#include "CrashHelper.hh"

#include <fmt/format.h>

namespace {
  static CrashHelper* static_instance = nullptr;
}

CrashHelper* CrashHelper::the() {
  if (static_instance == nullptr) {
    static_instance = new CrashHelper();
  }
  return static_instance;
}

CrashHelper::CrashHelper() = default;

void CrashHelper::verify_not_reached(std::string_view message) const {
  fmt::print("{}\n", message);
  exit(1);
}

void CrashHelper::verify(bool condition, std::string_view message) const {
  if (condition) {
    return;
  }
  fmt::print("{}\n", message);
  exit(1);
}

void CrashHelper::check_crash_observers_and_terminate() const {

}
