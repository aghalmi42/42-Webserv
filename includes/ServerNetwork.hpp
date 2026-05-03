
#ifndef SERVERNETWORK_HPP
# define SERVERNETWORK_HPP

#include <iostream>
#include <string>
#include <vector>
#include "ServerConfig.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <cstdlib>

class ServerNetwork
{
    private:
    std::vector<Server>   _server;
    std::vector<int>    _socket;

    public:
    ServerNetwork(std::vector<Server> server);
    ~ServerNetwork();
    ServerNetwork(const ServerNetwork& src);
    ServerNetwork& operator=(const ServerNetwork& src);
    void run();
};

#endif