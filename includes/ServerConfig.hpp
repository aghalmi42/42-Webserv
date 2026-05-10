
#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>

struct Location
{
    std::vector<std::string> methodHttp;
    std::string root;
    std::string redirection;
    std::string fileIndex;
    bool autoIndex;
    std::string uploadPath;
    std::string extensionCgi;
    std::string path;
};

struct Server
{
    std::string host;
    std::string port;
    std::string serverName;
    long MaxSizeClient;
    std::map<int, std::string> errorPage;
    std::map<std::string, Location> location;
};

#endif
