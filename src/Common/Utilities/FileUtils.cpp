//
// Created by Roger on 30/10/2024.
//
#include "Game/Utilities/FileUtils.h"
#include <iostream>
#include <fstream>
#include <sstream>


std::string LoadTextFile(const char* filename)
{
    std::ifstream file;
    std::stringstream buf;

    std::string ret;
    file.open(filename);
    if (file.is_open())
    {
        buf << file.rdbuf();
        ret = buf.str();
    }
    else
        std::cerr << "Failed to open file " << filename << std::endl;
    file.close();
    return ret;
}