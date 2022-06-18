#include "CrashHelper.hh"

#include <csignal>
#include <map>

#include "fmt/format.h"

namespace {
static CrashHelper* static_instance = nullptr;

[[noreturn]] void crash() {
    fmt::format("CRASH!!!!\n");
    std::abort();
}

void signal_handler(int signal) {
    CrashHelper::the()->on_crashy_signal(signal);
}

const std::map<int, std::string> crashy_signals = {
    { SIGSEGV, "SIGSEGV" },
    { SIGABRT, "SIGABRT" },
};
}  // namespace

CrashHelper::ScopedHandler::ScopedHandler(
    CrashHelper::crash_handler_callback_t callback) {
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
        fmt::print(
            "No crash handlers registered at call to "
            "CrashHelper::unregister_latest_handler()");
        crash();
    }
    m_crash_handlers.pop();
}

void CrashHelper::register_signals() {
    for (auto [signal, _] : crashy_signals) {
      std::signal(signal, signal_handler);
    }
}

void CrashHelper::unregister_signals() {
    for (auto [signal, _] : crashy_signals) {
      std::signal(signal, SIG_DFL);
    }
}

void CrashHelper::on_crashy_signal(int signal) {
    auto it = crashy_signals.find(signal);
    if (it != crashy_signals.end()) {
        fmt::print("Received signal {}\n", it->second);
    }
    check_crash_handler_and_terminate();
}

void CrashHelper::check_crash_handler_and_terminate() {
    unregister_signals();
    if (m_in_middle_of_crash_handling) {
        fmt::print("A crash happened during a handling of another crash, crashing immediately\n");
        crash();
    }
    m_in_middle_of_crash_handling = true;
    if (!m_crash_handlers.empty()) {
        m_crash_handlers.top()();
    }
    crash();
}
