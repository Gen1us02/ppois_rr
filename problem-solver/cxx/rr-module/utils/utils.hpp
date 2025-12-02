#pragma once

#include <vector>
#include <string>
#include <cstring>

// Класс вспомогательных методов
class Utils
{
public:
  // Метод, разбиения строки по разделителю
  static std::vector<std::string> SplitData(std::string & data, std::string const & sep);
  // Метод, получения индекса страницы
  static int GetPageIndex(std::string const & pageIdtf);
};