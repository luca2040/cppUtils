#pragma once

#include <string>

namespace utils
{
  enum StrColor
  {
    cNULL = 0,
    // normal colors
    cBLACK = 30,
    cRED = 31,
    cGREEN = 32,
    cYELLOW = 33,
    cBLUE = 34,
    cMAGENTA = 35,
    cCYAN = 36,
    cWHITE = 37,
    // bright colors
    cBBLACK = 90,
    cBRED = 91,
    cBGREEN = 92,
    cBYELLOW = 93,
    cBBLUE = 94,
    cBMAGENTA = 95,
    cBCYAN = 96,
    cBWHITE = 97,
  };

  enum BgColor
  {
    bNULL = 0,
    // normal colors
    bBLACK = 40,
    bRED = 41,
    bGREEN = 42,
    bYELLOW = 43,
    bBLUE = 44,
    bMAGENTA = 45,
    bCYAN = 46,
    bWHITE = 47,
    // bright colors
    bBBLACK = 100,
    bBRED = 101,
    bBGREEN = 102,
    bBYELLOW = 103,
    bBBLUE = 104,
    bBMAGENTA = 105,
    bBCYAN = 106,
    bBWHITE = 107,
  };

  enum StrEffect
  {
    eNULL = 0,
    // effects
    eBOLD = 1,
    eDIM = 2,
    eITALIC = 3,
    eUNDERLINE = 4,
    eBARRED = 9,
  };

  struct StrStyle
  {
    StrColor color = cNULL;
    BgColor bgColor = bNULL;
    StrEffect effect = eNULL;
    StrEffect effect2 = eNULL;

    StrStyle(StrColor color_,
             BgColor bgColor_,
             StrEffect effect_)
        : color(color_),
          bgColor(bgColor_),
          effect(effect_) {};

    StrStyle(StrColor color_,
             BgColor bgColor_)
        : color(color_),
          bgColor(bgColor_) {};

    StrStyle(StrColor color_,
             StrEffect effect_)
        : color(color_),
          effect(effect_) {};

    StrStyle(StrColor color_,
             StrEffect effect_,
             StrEffect effect2_)
        : color(color_),
          effect(effect_),
          effect2(effect2_) {};

    StrStyle(StrColor color_)
        : color(color_) {};

    StrStyle(StrEffect effect_)
        : effect(effect_) {};

    StrStyle(BgColor bgColor_)
        : bgColor(bgColor_) {};
  };

  static std::string operator<<(std::string_view str, const StrStyle &style)
  {
    if (style.color || style.bgColor || style.effect || style.effect2)
    {
      std::string effStr = "";

      if (style.color)
        effStr += "\033[" + std::to_string(style.color) + "m";
      if (style.bgColor)
        effStr += "\033[" + std::to_string(style.bgColor) + "m";
      if (style.effect)
        effStr += "\033[" + std::to_string(style.effect) + "m";
      if (style.effect2)
        effStr += "\033[" + std::to_string(style.effect2) + "m";

      return effStr + std::string(str) + "\033[0m";
    }

    return std::string(str);
  }
}