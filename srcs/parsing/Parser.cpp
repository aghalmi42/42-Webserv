
#include "../../includes/Parser.hpp"

Parser::Parser(std::string configPath) : _configPath(configPath) 
{

}

Parser::~Parser()
{
}

Parser::Parser(const Parser& src)
{
    *this = src;
}

Parser& Parser::operator=(const Parser& src)
{
    if (this != &src)
        this->_configPath = src._configPath;
    return (*this);
}

std::vector<Server> Parser::parser()
{
    std::vector<Server> vec;
    std::ifstream configFile(this->_configPath.c_str());
    if (!configFile.is_open())
    {
        std::cerr << "Error: could not open file"<< std::endl;
        return (vec);
    }
    std::string line;
    while(std::getline(configFile, line))
    {

    }
}
