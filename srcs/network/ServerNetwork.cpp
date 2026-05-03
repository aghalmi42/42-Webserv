
#include "../../includes/ServerNetwork.hpp"

ServerNetwork::ServerNetwork(std::vector<Server> server) : _server(server)
{
}

ServerNetwork::~ServerNetwork()
{
}

ServerNetwork::ServerNetwork(const ServerNetwork& src)
{
    *this = src;
}

ServerNetwork& ServerNetwork::operator=(const ServerNetwork& src)
{
    if (this != &src)
        this->_server = src._server;
    return (*this);
}

void ServerNetwork::run()
{
    int opt = 1;
    std::vector<Server>::iterator it = _server.begin();
    while(it != _server.end())
    {
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd == -1)
            return ;
        _socket.push_back(fd);
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        fcntl(fd, F_SETFL, O_NONBLOCK);
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(atoi(it->port.c_str()));
        addr.sin_addr.s_addr = INADDR_ANY;
        if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
            return ;
        if (listen(fd, 128) == -1)
            return ;
        it++;
    }
}
