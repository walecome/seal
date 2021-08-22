#pragma once

#include <optional>

template<class T>
class Result {
  public:
    static Result error() {
      return {};
    }
    static Result result(T&& t) {
      return Result(std::move(t));
    }
    
  bool is_error() const {
    return !m_result.has_value();
  }
  
  T&& get() {
    ASSERT(!is_error());
    return std::move(m_result.value());
  }

private:
  
  Result() : m_result(std::nullopt) {}
  Result(T&& t) : m_result(std::move(t)) {}

  Result(const Result&) = delete;

  std::optional<T> m_result;
};