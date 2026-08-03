#pragma once

// thread safe log stack implementation using deque from the other util

#include "threque.hpp"

#include <atomic>
#include <deque>
#include <string>

namespace utils
{

class LogStack
{
  private:
    std::atomic<size_t>              maxCount = {};
    threque<std::deque<std::string>> queue    = {};

    void clearToNumber()
    {
        while (queue.size() > maxCount)
            queue.pop_front();
    }

  public:
    LogStack(const size_t& maxCount_ = 50) : maxCount(maxCount_) {}
    ~LogStack() = default;

    void push(std::string str)
    {
        queue.push_back(std::move(str));
        clearToNumber();
    }

    const std::string get()
    {
        std::string ret = "";
        queue.foreach ([&](auto& val) { ret += val + "\n"; });
        return ret;
    }

    void setMaxLineNum(const size_t& num)
    {
        maxCount = num;
        clearToNumber();
    }

    void clear()
    {
        while (queue.size())
            queue.pop_front();
    }
};

} // namespace utils
