#include "argparse.hpp"

#include <algorithm>
#include <tuple>
#include <iostream>

void utils::argparse::ArgParser::displayHelpMenu()
{
  std::cout << ("             Help menu             " << titleStyle) << "\n"
            << std::endl;

  if (positionalArgs.size() > 0)
  {
    std::cout << ("Positional args:" << subtitleStyle) << std::endl;
    for (auto &positionalArg : positionalArgs)
    {
      std::cout << (("Position: " + std::to_string(positionalArg->position)) << helpArgStyle) << "\n    "
                << positionalArg->description << std::endl;

      if (&positionalArg != &positionalArgs.back())
        std::cout << std::endl;
    }
    std::cout << std::endl;
  }

  if (labeledArgs.size() > 0)
  {
    std::cout << ("Labeled args:" << subtitleStyle) << std::endl;
    for (auto &labeledArg : labeledArgs)
    {
      std::cout << (labeledArg->code.value_or("undefined") << helpArgStyle) << "\n    "
                << labeledArg->description << std::endl;

      if (labeledArg->possibleValues.has_value() &&
          (labeledArg->possibleValues.value().size() > 0))
      {
        std::cout << ("    Possible values: " << possibleValsStyle);
        for (auto &possibleVal : labeledArg->possibleValues.value())
        {
          std::cout << (possibleVal << valStyle);
          if (&possibleVal != &labeledArg->possibleValues.value().back())
            std::cout << ", ";
        }
        std::cout << std::endl;
      }

      if (&labeledArg != &labeledArgs.back())
        std::cout << std::endl;
    }
    std::cout << std::endl;
  }

  exit(0);
}

void utils::argparse::ArgParser::addArg(Arg *arg)
{
  if (arg->position < 0)
  { // labeled
    labeledArgs.push_back(arg);
  }
  else
  { // positional
    positionalArgs.push_back(arg);
  }
}

utils::argparse::ArgParser::ParsingError utils::argparse::ArgParser::parse()
{
  std::vector<std::pair<std::string, bool>> progArgs = {}; // arg, consumed
  for (int i = 0; i < argc; i++)
    progArgs.push_back({std::string(argv[i]), false});

  std::sort(positionalArgs.begin(), positionalArgs.end(),
            [](Arg *a, Arg *b)
            { return a->position < b->position; });

  for (size_t i = 0; i < progArgs.size(); i++)
  {
    auto &progArg = progArgs[i];
    if (progArg.second)
      continue;

    if (progArg.first == helpLabel)
      displayHelpMenu();

    for (auto &labeledArg : labeledArgs)
    {
      if (!labeledArg->code.has_value())
        continue;

      auto labArgStrCode = labeledArg->code.value();
      if (progArg.first == labArgStrCode)
      { // if it checks out
        if (labeledArg->result.found)
          return {.code = ARG_DUPLICATE,
                  .errorMessage = (labArgStrCode << argStyle) + " " + argDuplicated};

        if (labeledArg->parseValue)
        { // if it requires parsing the next one
          do
          {
            i++;
          } while (i < progArgs.size() && progArgs[i].second);
          if (i >= progArgs.size())
            return {.code = ARG_REQUIRES_VAL,
                    .errorMessage = (labArgStrCode << argStyle) + " " + argReqVal};

          auto &nextprogArg = progArgs[i];

          if (labeledArg->possibleValues.has_value())
          {
            bool valuePossible = false;
            for (auto &possibleValue : labeledArg->possibleValues.value())
            {
              if (nextprogArg.first == possibleValue)
              {
                valuePossible = true;
                break;
              }
            }

            if (!valuePossible)
            {
              return {.code = ARG_INVALID_VALUE,
                      .errorMessage = (labArgStrCode << argStyle) + " " + argNotOption + " " + (nextprogArg.first << argStyle)};
            }
          }

          nextprogArg.second = true;
          labeledArg->result.found = true;
          labeledArg->result.parsedVal = nextprogArg.first;
        }
        else
        { // if it doesnt require parsing
          labeledArg->result.found = true;
        }

        progArg.second = true;
        break;
      }
    }
  }

  for (size_t i = 0; i < progArgs.size(); i++)
  {
    auto &progArg = progArgs[i];
    if (progArg.second)
      continue;

    for (auto &positionalArg : positionalArgs)
    {
      if (positionalArg->result.found)
        continue;

      progArg.second = true;
      positionalArg->result.found = true;
      positionalArg->result.parsedVal = progArg.first;
      break;
    }
  }

  std::vector<std::string> resLeftovers = {};
  for (size_t i = 0; i < progArgs.size(); i++)
  {
    auto &progArg = progArgs[i];
    if (!progArg.second)
      resLeftovers.push_back(progArg.first);
  }

  if (resLeftovers.size() <= 0)
    return {};

  std::string leftoverStr = {};
  for (auto &leftover : resLeftovers)
  {
    leftoverStr += (leftover << valStyle);
    if (&leftover != &resLeftovers.back())
      leftoverStr += ", ";
  }

  return {.leftovers = leftoverStr};
}