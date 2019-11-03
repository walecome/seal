#pragma once

#include <cassert>
#include <memory>
#include <sstream>
#include <variant>

#define ASSERT_NOT_REACHED() assert(false)
#define ASSERT assert

static std::ostringstream out {};

template <typename T>
using ptr_t = std::unique_ptr<T>;

template <typename T>
using sptr_t = std::shared_ptr<T>;

using expr_value_t = std::variant<int, float, bool, std::string>;
