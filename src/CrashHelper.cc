#include "CrashHelper.hh"

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

void CrashHelper::check_crash_handler_and_terminate() const {

}
