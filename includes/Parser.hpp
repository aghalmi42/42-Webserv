
#ifndef PARSER_HPP
# define PARSER_HPP

#include <iostream>
#include <string>
#include <vector>
#include "ServerConfig.hpp"
#include <fstream>

class Parser
{
    private:
    std::string _configPath;

    public:
    Parser(std::string configPath);
    ~Parser();
    Parser(const Parser& src);
    Parser& operator=(const Parser& src);

    std::vector<Server> parse();
};

#endif