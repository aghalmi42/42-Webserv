
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

std::vector<Server> Parser::parse()
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
        if (line.find("server") != std::string::npos && line.find("{") != std::string::npos)
            vec.push_back(parserServer(configFile));
    }
    return (vec);
}

Server Parser::parserServer(std::ifstream& readLine)
{
    Server server;
    std::string line;
    while(std::getline(readLine, line))
    {
        line = strTrim(line);
         if (line == "}")
           break ;
        if (line[0] == '#')
            continue ;
        if (line.find("listen") != std::string::npos)
        {
            size_t begin = line.find("listen") + 7;
            size_t end = line.find(";");
            std::string extract = line.substr(begin, end - begin);
            extract = strTrim(extract);
            server.port = extract;
        }
        if (line.find("server_name") != std::string::npos)
        {
            size_t begin = line.find("server_name") + 12;
            size_t end = line.find(";");
            std::string extract = line.substr(begin, end - begin);
            extract = strTrim(extract);
            server.serverName = extract; 
        }
        if (line.find("client_max_body_size") != std::string::npos)
        {
            size_t begin = line.find("client_max_body_size") + 21;
            size_t end = line.find(";");
            std::string extract = line.substr(begin, end - begin);
            extract = strTrim(extract);
            server.MaxSizeClient = atol(extract.c_str());
        }
        if (line.find("error_page") != std::string::npos)
        {
            size_t begin = line.find("error_page") + 11;
            size_t end = line.find(";");
            std::string extract = line.substr(begin, end - begin);
            extract = strTrim(extract);
            std::string space = extract.substr(0, extract.find(" "));
            std::string path = extract.substr(extract.find(" ") + 1);
            server.errorPage[atoi(space.c_str())] = path;
        }
        if (line.find("location") != std::string::npos)
        {
            size_t begin = line.find("location") + 9;
            size_t end = line.find("{");
            std::string extractPath = line.substr(begin, end - begin);
            extractPath = strTrim(extractPath);
            server.location[extractPath] = parserLocation(readLine, extractPath);
        }
    }
    return (server);
}

Location Parser::parserLocation(std::ifstream& readLine, std::string pathLocation)
{
    Location location;
    location.path = pathLocation;
    std::string line;
    while(std::getline(readLine, line))
    {
        line = strTrim(line);
        if (line == "}")
            break ;
        if (line[0] == '#')
            continue ;
        if (line.find("methods") != std::string::npos)
        {
            size_t begin = line.find("methods") + 8;
            size_t end = line.find(";");
            std::string extract = line.substr(begin, end - begin);
            std::istringstream stream(extract);
            std::string method;
            while(stream >> method)
                location.methodHttp.push_back(method);
        }
        if (line.find("root") != std::string::npos)
        {
            size_t begin = line.find("root") + 5;
            size_t end = line.find(";");
            std::string extract = line.substr(begin, end - begin);
            extract = strTrim(extract);
            location.root = extract;
        }
        if (line.find("redirection") != std::string::npos)
        {
            size_t begin = line.find("redirection") + 12;
            size_t end = line.find(";");
            std::string extract = line.substr(begin, end - begin);
            extract = strTrim(extract);
            location.redirection = extract;
        }
        if (line.find("index") != std::string::npos && line.find("autoindex") == std::string::npos)
        {
            size_t begin = line.find("index") + 6;
            size_t end = line.find(";");
            std::string extract = line.substr(begin, end - begin);
            extract = strTrim(extract);
            location.fileIndex = extract;
        }
        if (line.find("autoindex") != std::string::npos)
        {
            size_t begin = line.find("autoindex") + 10;
            size_t end = line.find(";");
            std::string extract = line.substr(begin, end - begin);
            extract = strTrim(extract);
            if (extract == "on")
                location.autoIndex = true;
            else
                location.autoIndex = false;
        }
        if (line.find("uploadPath") != std::string::npos)
        {
            size_t begin = line.find("uploadPath") + 11;
            size_t end = line.find(";");
            std::string extract = line.substr(begin, end - begin);
            extract = strTrim(extract);
            location.uploadPath = extract;
        }
        if (line.find("extensionCgi") != std::string::npos)
        {
            size_t begin = line.find("extensionCgi") + 13;
            size_t end = line.find(";");
            std::string extract = line.substr(begin, end - begin);
            extract = strTrim(extract);
            location.extensionCgi = extract;
        }
    }
    return (location);
}
