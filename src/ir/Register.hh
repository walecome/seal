#pragma once

#include <fmt/format.h>
#include <string>
#include "fmt/core.h"

class Register {
   public:
    Register();
    explicit Register(unsigned index);

    bool is_used() const;
    unsigned index() const;
    std::string to_string() const;

   private:
    const unsigned m_index;
};