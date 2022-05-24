#pragma once

#include <string_view>

class CrashHandler {
  public:
    static CrashHandler* the();

  void verify(bool condition, std::string_view message);
  [[noreturn]] void verify_not_reached(std::string_view message);

  private:
    CrashHandler();
};
