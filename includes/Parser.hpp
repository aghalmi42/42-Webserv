
#ifndef PARSER_HPP
# define PARSER_HPP

#include <iostream>
#include <string>
#include <vector>
#include "ServerConfig.hpp"
#include <fstream>
#include <cstdlib>
#include <sstream>

class Parser
{
    private:
    std::string _configPath;
    Server parserServer(std::ifstream& readLine);
    Location parserLocation(std::ifstream& readLine, std::string pathLocation);

    public:
    Parser(std::string configPath);
    ~Parser();
    Parser(const Parser& src);
    Parser& operator=(const Parser& src);

    std::vector<Server> parse();
};

#endif