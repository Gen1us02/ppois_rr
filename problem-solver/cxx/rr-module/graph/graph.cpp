#include "graph.hpp"
#include <stdexcept>

void Graph::AddVertex(std::shared_ptr<Vertex> start_vertex, const std::string& dest_vertex){
    if (start_vertex->page == dest_vertex)
        throw std::invalid_argument("Same page");

    if (graph.find(start_vertex) == graph.end())
        graph[start_vertex] = std::vector<std::string>();

    graph[start_vertex].push_back(dest_vertex);
}

std::map<std::shared_ptr<Vertex>, std::vector<std::string>> Graph::GetGraph() const{
    return graph;
}