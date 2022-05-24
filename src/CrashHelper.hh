#pragma once

#include <string_view>
#include <functional>
#include <stack>

class CrashHelper {
  public:
    using crash_handler_callback_t = std::function<void()>;

    class ScopedHandler {
      public:
        explicit ScopedHandler(crash_handler_callback_t callback);
        ~ScopedHandler();
    };

    static CrashHelper* the();

    void register_crash_handler(crash_handler_callback_t callback);
    void unregister_latest_handler();

  [[noreturn]] void check_crash_handler_and_terminate() const;

  private:
    CrashHelper();

    std::stack<crash_handler_callback_t> m_crash_handlers;
};
