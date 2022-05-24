#include <string>

#include "fmt/format.h"

#include "Constants.hh"
#include "CrashHelper.hh"

void verify_not_reached(const char* file, int line) {
    std::string message = fmt::format(
        "At location {}:{}: Should not have been reached!", file, line);
    CrashHelper::the()->verify_not_reached(message);
}

void verify_not_reached_message(const char* file, int line,
                                const std::string& message) {
    verify_not_reached_message(file, line, message.c_str());
}

void verify_not_reached_message(const char* file, int line,
                                const char* message) {
    std::string full_message =
        fmt::format("At location {}:{}: Should not have been reached: {}", file,
                    line, message);
    CrashHelper::the()->verify_not_reached(full_message);
}

void verify(bool condition, const char* file, int line,
            const char* expression) {
    std::string message = fmt::format(
        "At location {}:{}: Assertion '{}' failed", file, line, expression);
    CrashHelper::the()->verify(condition, message);
}
