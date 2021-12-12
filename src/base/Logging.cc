#include "base/Logging.hh"

#include <fmt/format.h>

namespace base {
    void log(std::string_view msg) {
        fmt::print("sealc: {}\n", msg);
    }
}
