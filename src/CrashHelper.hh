#pragma once

#include <string_view>

class CrashHelper {
  public:
    static CrashHelper* the();

  [[noreturn]] void check_crash_handler_and_terminate() const;

  private:
    CrashHelper();

};
