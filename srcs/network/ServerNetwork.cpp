
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
    std::vector<int>::iterator itt = _socket.begin();
    while(itt != _socket.end())
    {
        struct pollfd pfd;
        pfd.events = POLLIN;
        pfd.revents = 0;
        pfd.fd = *itt;
        this->_fds.push_back(pfd);
        itt++;
    }
    while(true)
    {
        int returN = poll(this->_fds.data(), this->_fds.size(), -1);
        if (returN == -1)
            return ;
        std::vector<struct pollfd>::iterator itfd = this->_fds.begin();
        while(itfd != this->_fds.end())
        {
            if (itfd->revents & POLLIN)
            {
                if (std::find(_socket.begin(), _socket.end(), itfd->fd) != _socket.end())
                {
                    acceptClient(itfd->fd);
                    break ;
                }
                else
                {
                    handleClient(itfd->fd);
                    break ;
                }
            }
            itfd++;
        }
    }
}

void ServerNetwork::acceptClient(int fd)
{
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    int clientFd = accept(fd, (struct sockaddr*)&client, &len);
    if (clientFd == -1)
        return ;
    fcntl(clientFd, F_SETFL, O_NONBLOCK);
    struct pollfd pfd;
    pfd.fd = clientFd;
    pfd.events = POLLIN;
    pfd.revents = 0;
    this->_fds.push_back(pfd);
}

void ServerNetwork::handleClient(int fd)
{
    char buff[4096];
    ssize_t handleFd = recv(fd, buff, sizeof(buff), 0);
    if (handleFd == 0)
        removeClient(fd);
    else if (handleFd == -1)
    {
        std::cerr << "Error: recv" << std::endl;
        removeClient(fd);
    }
    else
    {
        std::string request = std::string(buff, handleFd);
		Routing router;
		Location loc = router.traitement(request, _server[0]);
		Response res(request, _server[0], loc);
		res.build();
		send(fd, res.get().c_str(), res.get().size(), 0);
    }
}

void ServerNetwork::removeClient(int fd)
{
    close(fd);
        std::vector<struct pollfd>::iterator it = _fds.begin();
        while(it != _fds.end())
        {
            if (it->fd == fd)
            {
                _fds.erase(it);
                break ;
            }
            it++;
        }
}
