#include "utils.hpp"
#include <stdexcept>

std::vector<std::string> Utils::SplitData(std::string & data, std::string const & sep)
{
  std::vector<std::string> result;
  if (data.empty())
  {
    return result;
  }

  char * token = std::strtok(data.data(), sep.c_str());
  while (token)
  {
    result.push_back(token);
    token = std::strtok(nullptr, sep.c_str());
  }

  return result;
}

int Utils::GetPageIndex(std::string const & pageIdtf)
{
  size_t i = pageIdtf.size() - 1;
  while (i > 0 && pageIdtf[i] != '_')
  {
    i--;
  }
  if (pageIdtf[i] == '_')
  {
    return std::stoi(pageIdtf.substr(i + 1));
  }

  throw std::invalid_argument("Invalid pageIdtf format");
}