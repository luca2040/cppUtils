#pragma once

#include <optional>
#include <vector>
#include <string>

#include "../colorizestr.hpp"

namespace utils::argparse
{
  struct ArgParseRes
  {
    bool found;
    std::optional<std::string> parsedVal;

    ArgParseRes() : found(false), parsedVal({}) {}
    ArgParseRes(const std::optional<std::string> &defaultVal_) : found(false), parsedVal(defaultVal_) {}
  };
  struct Arg
  {
    std::optional<std::string> code;
    int position;

    bool parseValue;
    std::optional<std::vector<std::string>> possibleValues;

    ArgParseRes result;

    std::string_view description;

    Arg(const std::optional<std::string> &code_,
        int position_ = -1, // position in the args not including tagged ones
        bool parseValue_ = false,
        const std::optional<std::vector<std::string>> &possibleValues_ = {},
        const std::optional<std::string> &defaultVal_ = {})
        : code(code_),
          position(position_),
          parseValue(parseValue_),
          possibleValues(possibleValues_),
          result({defaultVal_}),
          description("----") {};
  };

  class ArgParser
  {
  private:
    int argc;
    char **argv;

    bool hasHelpMenu = false;

    std::vector<Arg *> labeledArgs = {};
    std::vector<Arg *> positionalArgs = {};

    // help menu
    static inline const StrStyle titleStyle = StrStyle{cBLACK, bBGREEN, eBOLD};
    static inline const StrStyle subtitleStyle = StrStyle{cBBLUE, eBOLD};
    static inline const StrStyle possibleValsStyle = StrStyle{cYELLOW, eBOLD};
    static inline const StrStyle valStyle = StrStyle{cBLACK, bBCYAN, eBOLD};
    static inline const StrStyle helpArgStyle = StrStyle{cBLACK, bBBLACK, eBOLD};
    static inline const std::string helpLabel = "--help";
    void displayHelpMenu();

    // error handling
    static inline const char *argNotOption = "does not have the option";
    static inline const char *argDuplicated = "duplicated";
    static inline const char *argReqVal = "requires a value";
    static inline const StrStyle argStyle = StrStyle{cBMAGENTA, eBOLD};

  public:
    // error handling again
    enum ParseErrorCode
    {
      OK,
      ARG_INVALID_VALUE,
      ARG_DUPLICATE,
      ARG_REQUIRES_VAL
    };
    struct ParsingError
    {
      ParseErrorCode code = OK;
      std::optional<std::string> leftovers = std::nullopt;
      std::optional<std::string> errorMessage = std::nullopt;
    };

    ArgParser(int argc_, char **argv_) : argc(argc_),
                                         argv(argv_) {};

    inline void enableHelpMenu() { hasHelpMenu = true; };

    void addArg(Arg *arg);
    ParsingError parse();
  };
}