#pragma once

#include <functional>
#include <stack>
#include <string_view>

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
    void register_signals();
    void unregister_signals();
    void on_crashy_signal(int signal);

    [[noreturn]] void check_crash_handler_and_terminate();

   private:
    CrashHelper();

    std::stack<crash_handler_callback_t> m_crash_handlers;
    bool m_in_middle_of_crash_handling { false };
};
