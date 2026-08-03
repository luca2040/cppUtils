// utils/converter/converter.cpp from https://github.com/luca2040/cppUtils

#include "converter.hpp"

#include <string> // yeah this is no duplicated shit. both needed
#include <string.h>

bool utils::converter::Converter::strToULong(const char *input, unsigned long &outValue)
{
  size_t inputLen = strlen(input);
  if (!inputLen)
    return false;

  unsigned long retValue = 0;

  // bin
  if (inputLen > 2 &&
      input[0] == '0' &&
      (input[1] == 'b' || input[1] == 'B'))
  {
    for (size_t i = 2; i < inputLen; i++)
    {
      char currentChr = input[i];
      if (currentChr != '0' && currentChr != '1')
        return false;

      retValue = (retValue << 1) + (currentChr - '0'); // because '0' + 1 = '1'
    }
  }
  else
  {
    // skip zeroes cuz they make strtoul go oct
    while (inputLen > 1 && input[0] == '0' && input[1] != 'x')
    {
      input++;
      inputLen--;
    }

    size_t endedIdx = 0;
    // dec and hex
    try
    {
      retValue = std::stoul(input, &endedIdx, 0);
    }
    catch (...)
    {
      return false;
    }

    if (endedIdx != inputLen)
      return false;
  }

  outValue = retValue;
  return true;
}
