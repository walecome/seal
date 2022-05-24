#include <string>

#include "fmt/format.h"

#include "Constants.hh"
#include "CrashHelper.hh"

namespace {
static const char* program_name = "sealc";

[[noreturn]] void print_and_terminate(const char* file, int line, std::string_view message) {
    fmt::print("{} {}:{}: {}\n", program_name, file, line, message);
    CrashHelper::the()->check_crash_handler_and_terminate();
}
}  // namespace

void verify_not_reached(const char* file, int line) {
    print_and_terminate(file, line, "SHOULD NOT BE REACHED!");
}

void verify_not_reached_message(const char* file, int line,
                                const std::string& message) {
    verify_not_reached_message(file, line, message.c_str());
}

void verify_not_reached_message(const char* file, int line,
                                const char* message) {
    print_and_terminate(file, line, fmt::format("SHOULD NOT BE REACHED: {}", message));
}

void verify(bool condition, const char* file, int line,
            const char* expression) {
    if (condition) {
        return;
    }
    print_and_terminate(file, line, fmt::format("Assertion '{}' failed", expression));
}
