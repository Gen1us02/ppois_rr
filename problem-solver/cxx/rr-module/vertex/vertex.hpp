#pragma once

#include <string>


struct Vertex
{
    std::string page;
    int visitors;
    int spent_time;

    Vertex(const std::string& page, int visitors, int spent_time);
};