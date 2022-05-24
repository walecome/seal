#include "CrashHelper.hh"

#include "fmt/format.h"

namespace {
  static CrashHelper* static_instance = nullptr;

  [[noreturn]] void crash() {
    throw 1;
  }
}

CrashHelper::ScopedHandler::ScopedHandler(CrashHelper::crash_handler_callback_t callback) {
  CrashHelper::the()->register_crash_handler(callback);
}

CrashHelper::ScopedHandler::~ScopedHandler() {
  CrashHelper::the()->unregister_latest_handler();
}

CrashHelper* CrashHelper::the() {
  if (static_instance == nullptr) {
    static_instance = new CrashHelper();
  }
  return static_instance;
}

CrashHelper::CrashHelper() = default;

void CrashHelper::register_crash_handler(crash_handler_callback_t callback) {
  m_crash_handlers.push(callback);
}

void CrashHelper::unregister_latest_handler() {
  if (m_crash_handlers.empty()) {
    fmt::print("No crash handlers registered at call to CrashHelper::unregister_latest_handler()");
    crash();
  }
  m_crash_handlers.pop();
}

void CrashHelper::check_crash_handler_and_terminate() const {
  if (!m_crash_handlers.empty()) {
    m_crash_handlers.top()();
  }
  crash();
}
