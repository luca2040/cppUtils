// thread safe deque-compatible template class

#pragma once

#include <mutex>
#include <optional>
#include <shared_mutex>

namespace utils
{

template <typename T> class threque
{
  private:
    std::shared_mutex mut;
    T                 value;

  public:
    threque() : value({}) {}
    threque(const T& value_) : value(value_) {}
    ~threque() = default;

    // modifiers
    void push_back(auto&& element)
    {
        std::unique_lock lock(mut);
        value.push_back(element);
    }

    void pop_back()
    {
        std::unique_lock lock(mut);
        value.pop_back();
    }
    std::optional<typename T::value_type> try_pop_back()
    {
        std::unique_lock lock(mut);

        if (value.empty())
            return std::nullopt;

        auto v = std::move(value.back());
        value.pop_back();
        return v;
    }

    void push_front(auto&& element)
    {
        std::unique_lock lock(mut);
        value.push_front(element);
    }

    void pop_front()
    {
        std::unique_lock lock(mut);
        value.pop_front();
    }
    std::optional<typename T::value_type> try_pop_front()
    {
        std::unique_lock lock(mut);

        if (value.empty())
            return std::nullopt;

        auto v = std::move(value.front());
        value.pop_front();
        return v;
    }

    void set(size_t idx, auto&& val)
    {
        std::unique_lock lock(mut);
        value[idx] = val;
    }
    void clear()
    {
        std::unique_lock lock(mut);
        value.clear();
    }

    // size
    size_t size()
    {
        std::shared_lock lock(mut);
        return value.size();
    }

    // iteration
    void foreach (auto&& func)
    {
        std::shared_lock lock(mut);
        for (auto& element : value)
            func(element);
    }
    auto operator[](const size_t& idx)
    {
        std::shared_lock lock(mut);
        return value[idx];
    }
};

} // namespace utils
