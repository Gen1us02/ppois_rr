#pragma once

#include "../vertex/vertex.hpp"
#include <map>
#include <vector>
#include <memory>

class Graph{
public:
    Graph();
    ~Graph();
    void AddVertex(std::shared_ptr<Vertex> start_vertex, const std::string& dest_vertex);
    std::map<std::shared_ptr<Vertex>, std::vector<std::string>> GetGraph() const;
private:
    std::map<std::shared_ptr<Vertex>, std::vector<std::string>> graph;
};