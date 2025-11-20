#pragma once

#include <string>
#include <vector>
#include <cstring>
#include <fstream>
#include <sstream>
#include "../graph/graph.hpp"

class Utils{
public:
    static void readCSV(const std::string& filename, Graph& graph);
    static std::vector<std::string> split(std::string& line);
};