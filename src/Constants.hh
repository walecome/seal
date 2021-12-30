#pragma once

#include <cassert>
#include <memory>
#include <sstream>

bool print_assert_message(const char* message);

bool print_assert_message(const std::string& message);

#define ASSERT_NOT_REACHED() assert(false)
#define ASSERT_NOT_REACHED_MSG(message) assert(print_assert_message(message) && false)
#define ASSERT assert
#define NOT_IMPLEMENTED() assert("Not implemented" && false)

static std::ostringstream out {};

template <typename T>
using ptr_t = std::unique_ptr<T>;

template <typename T>
using sptr_t = std::shared_ptr<T>;

#define MAKE_DEFAULT_CONSTRUCTABLE(c) \
   public:                            \
    c() = default;

#define MAKE_DEFAULT_COPYABLE(c) \
   public:                       \
    c(const c&) = default;       \
    c& operator=(const c&) = default;

#define MAKE_DEFAULT_MOVABLE(c) \
   public:                      \
    c(c&&) = default;           \
    c& operator=(c&&) = default;

#define MAKE_NONCOPYABLE(c) \
   private:                 \
    c(const c&) = delete;   \
    c& operator=(const c&) = delete;

#define MAKE_NONMOVABLE(c) \
   private:                \
    c(c&&) = delete;       \
    c& operator=(c&&) = delete;
