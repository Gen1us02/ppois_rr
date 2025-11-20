#include "utils.hpp"

std::vector<std::string> Utils::split(std::string& line){
    std::vector<std::string> result;
    char* token = std::strtok(line.data(), ",");
    while(token != nullptr){
        result.push_back(token);
        token = std::strtok(nullptr, ",");
    }

    return result;
}

void Utils::readCSV(const std::string& filename, Graph& graph){
    std::ifstream file(filename);

    if(!file.is_open()){
        throw std::runtime_error("File didn't opened");
    }

    std::string line;
    while(std::getline(file, line)){
        std::string page_name;
        std::string str_visitors;
        std::string str_time;
        std::string link_list;
        int visitors, time;
        std::vector<std::string> page_links;

        std::stringstream ss(line);
        if( std::getline(ss, page_name, ';') &&
            std::getline(ss, str_visitors, ';') &&
            std::getline(ss, str_time, ';') &&
            std::getline(ss, link_list, ';')){
            
            visitors = std::stoi(str_visitors);
            time = std::stoi(str_time);
            if(!link_list.empty()){
                page_links = split(link_list);;
            }
        }

        std::shared_ptr<Vertex> vertex = std::make_shared<Vertex>(page_name, visitors, time);
        for (const auto& link: page_links){
            graph.AddVertex(vertex, link);
        }
    }
}