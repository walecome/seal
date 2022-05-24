#pragma once

#include <cassert>
#include <memory>
#include <sstream>

[[noreturn]] void verify_not_reached(const char* file, int line);
[[noreturn]] void verify_not_reached_message(const char* file, int line,
                                             const char* message);
[[noreturn]] void verify_not_reached_message(const char* file, int line,
                                             const std::string& message);
void verify(bool condition, const char* file, int line, const char* expression);

#define ASSERT_NOT_REACHED() verify_not_reached(__FILE__, __LINE__)
#define ASSERT_NOT_REACHED_MSG(message) \
    verify_not_reached_message(__FILE__, __LINE__, message)
#define ASSERT(condition) verify(condition, __FILE__, __LINE__, #condition)

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
