#pragma once

#include <memory>
#include <sstream>

static std::ostringstream out {};

template <typename T>
using ptr_t = std::unique_ptr<T>;
