#pragma once

#include <fmt/format.h>
#include <string>
#include "fmt/core.h"

#include "Register.hh"

namespace {
  unsigned UNUSED_REGISTER_INDEX = -1;
}

Register::Register() : m_index(UNUSED_REGISTER_INDEX) {}
Register::Register(unsigned index) : m_index(index) {}

bool Register::is_used() const {
  return index() != UNUSED_REGISTER_INDEX;
}

unsigned Register::index() const { return m_index; }

std::string Register::to_string() const { return fmt::format("r{}", m_index); }
