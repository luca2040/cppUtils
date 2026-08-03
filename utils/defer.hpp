#pragma once

#include <functional>

// the simplest defer implementation i could think of.

namespace utils
{

class defer
{
  private:
    std::function<void()> operation;

  public:
    defer() : operation([] {}) {};
    defer(const std::function<void()>& operation_) : operation(operation_) {};

    void set(const std::function<void()>& operation_) { operation = operation_; }

    ~defer() { operation(); };
};

#define defer_(name, function) utils::defer name([&] function)
#define undefer_(name) name.set([] {})

} // namespace utils
