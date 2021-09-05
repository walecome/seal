#include <string>

#include "fmt/core.h"
#include "fmt/format.h"

#include "Constants.hh"

bool print_assert_message(const std::string& message) {
   return print_assert_message(message.c_str());
}

bool print_assert_message(const char* message) {
   fmt::print(fmt::format("{}\n", message));
   return false;
}