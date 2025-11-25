#include "utils.hpp"

std::vector<std::string> Utils::splitData(std::string& data, const std::string& sep){
    std::vector<std::string> result;
    if (data.empty()){
        return result;
    }
    
    char* token = std::strtok(data.data(), sep.c_str());
    while(token){
        result.push_back(token);
        token = std::strtok(nullptr, sep.c_str());
    }

    return result;
}