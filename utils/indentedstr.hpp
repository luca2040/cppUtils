#pragma once

#include <string>

namespace utils
{
  class IndentedStr
  { // this may be not optimized or something, i dont care
  private:
    std::string str = {};
    unsigned int indSpaces = 4;
    unsigned int indentLevel = 0;
    bool applyIndent = true;

  public:
    IndentedStr(const std::string &str_ = {},
                const unsigned int &indentLevel_ = 0,
                const unsigned int &indSpaces_ = 4,
                const bool &applyIndent_ = true)
        : str(str_),
          indentLevel(indentLevel_),
          indSpaces(indSpaces_),
          applyIndent(applyIndent_) {}

    inline unsigned int pushInd()
    {
      return ++indentLevel;
    }
    inline unsigned int popInd()
    {
      return --indentLevel;
    }
    inline void enableInd()
    {
      applyIndent = true;
    }
    inline void disableInd()
    {
      applyIndent = false;
    }

    void addInd()
    {
      if (applyIndent)
        str.append(indentLevel * indSpaces, ' ');
    }

    inline void operator+=(const std::string &__str)
    {
      str += __str;
    }
    inline std::string &stdstr()
    {
      return str;
    }
  };
}