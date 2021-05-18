//
// Created by umar on 5/18/21.
//

#ifndef DATA_TYPES_V0_0_0_VALIDATOR_H
#define DATA_TYPES_V0_0_0_VALIDATOR_H

#include <string>
namespace Validator
{
static bool isBase16(const std::string& value)
{
   if (value.c_str() && std::regex_match(value, std::regex("^(0x)?[a-f0-9]+$")))
      return true;
   else
      return false;
}
}


#endif    // DATA_TYPES_V0_0_0_VALIDATOR_H
