#pragma once

#include <vector>
#include <string>
#include <cstring>

class Utils
{
public:
  static std::vector<std::string> splitData(std::string & data, std::string const & sep);
};