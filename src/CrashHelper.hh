#pragma once

#include <string_view>

class CrashHelper {
  public:
    static CrashHelper* the();

  void verify(bool condition, std::string_view message) const;
  [[noreturn]] void verify_not_reached(std::string_view message) const;

  private:
    CrashHelper();

    void check_crash_observers_and_terminate() const;
};
