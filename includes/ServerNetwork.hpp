
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
#include <poll.h>
#include <algorithm>
#include <unistd.h>
#include "Routing.hpp"
#include <sstream>
#include <fstream>
#include <sys/wait.h>
#include "Response.hpp"

class ServerNetwork
{
    private:
    std::vector<Server>   _server;
    std::vector<int>    _socket;
    void acceptClient(int fd);
    std::vector<struct pollfd> _fds;
    void handleClient(int fd);
    void removeClient(int fd);
    Server getServer(std::string request);

    public:
    ServerNetwork(std::vector<Server> server);
    ~ServerNetwork();
    ServerNetwork(const ServerNetwork& src);
    ServerNetwork& operator=(const ServerNetwork& src);
    void run();
};

#endif