#pragma once

#include <memory>

template <typename T>
using ptr_t = std::unique_ptr<T>;
