#pragma once

#include <vector>
#include <string>
#include <cstring>

class Utils
{
public:
  static std::vector<std::string> SplitData(std::string & data, std::string const & sep);
  static int GetPageIndex(std::string const & pageIdtf);
};