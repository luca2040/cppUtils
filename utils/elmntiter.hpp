#pragma once

#include <optional>
#include <vector>
#include <functional>

namespace utils
{
  template <typename Output>
  class ElementIterator
  {
  private:
    Output data = {};
    size_t currentIdx = 0;
    std::vector<size_t> checkpointStack = {};
    std::function<void()> onTrash = {};

  public:
    inline Output &getData()
    {
      return data;
    }
    inline std::function<void()> &getTrashFunc()
    {
      return onTrash;
    }

    // ######################

    inline bool hasValue(size_t offset = 0)
    {
      return (currentIdx + offset) < data.size();
    }

    inline std::optional<typename Output::value_type> read(size_t offset = 0)
    {
      if (hasValue(offset))
        return data[currentIdx + offset];
      return std::nullopt;
    }

    inline void trash(size_t number = 1)
    {
      size_t targetIdx = currentIdx + number;

      if (onTrash)
        for (; currentIdx < targetIdx; currentIdx++)
          onTrash();
      else
        currentIdx = targetIdx;
    }

    inline void setCheckpoint()
    {
      checkpointStack.push_back(currentIdx);
    }

    inline void restoreCheckpoint()
    {
      currentIdx = checkpointStack.back();
      checkpointStack.pop_back();
    }

    inline void popCheckpoint()
    {
      checkpointStack.pop_back();
    }
  };
}