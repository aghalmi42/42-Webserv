
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
		// Jai changeer quelque truc stv retrouver ton code de base tu supprime juste le //add by adel et tu decommante tout
        std::string request = std::string(buff, handleFd);
		Routing router; // add by adel
		Location loc = router.traitement(request, _server[0]); // add by adel
		Response res(request, _server[0], loc); // add by adel
		res.build();
		send(fd, res.get().c_str(), res.get().size(), 0);

        // size_t first = request.find(" ");
        // size_t begin = first + 1;
        // size_t end = request.find(" ", begin);
        // std::string url = request.substr(begin, end - begin);
        // std::string method = request.substr(0, request.find(" "));
        // if (method == "GET")
        // {
        //     Routing router;
        //     Location loc = router.traitement(request, _server[0]);
        //     handleGet(fd, url, loc); 
        // }
        // else if (method == "POST")
        // {
        //     Routing router;
        //     Location loc = router.traitement(request, _server[0]);
        //     size_t start = request.find("\r\n\r\n");
        //     std::string body = request.substr(start + 4);
        //     handlePost(fd, url, body, loc);
        // }
        // else if (method == "DELETE")
        // {
        //     Routing router;
        //     Location loc = router.traitement(request, _server[0]);
        //     handleDelete(fd, url, loc);
        // }
        // else
        // {
        //     std::string response = "HTTP/1.1 405 Method not allowed\r\nContent-Length: 0\r\n\r\n";
        //     send(fd, response.c_str(), response.size(), 0);
        // }
	

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

std::string ServerNetwork::getFile(Location loc, std::string url)
{
    std::string filePath = loc.root + url;
    if (url == "/")
        filePath = loc.root + "/" + loc.fileIndex;
    std::ifstream openFile(filePath.c_str());
    if (!openFile.is_open())
    {
        std::cerr << "Error: could not open file"<< std::endl;
        return ("");
    }
    std::stringstream ss;
    ss << openFile.rdbuf();
    return (ss.str());
}

// void ServerNetwork::handleGet(int fd, std::string url, Location loc)
// {
//         std::string body = getFile(loc, url);
//         if (body.empty())
//         {
//             std::ifstream errorFile("www/404.html");
//             std::stringstream ss;
//             ss << errorFile.rdbuf();
//             std::string errorBody = ss.str();
//             std::stringstream ss2;
//             ss2 << errorBody.size();
//             std::string response = "HTTP/1.1 404 Not Found\r\nContent-Length: " + ss2.str() + "\r\n\r\n" + errorBody;
//             send(fd, response.c_str(), response.size(), 0);
//         }
//         else
//         {
//             std::stringstream ss;
//             ss << body.size();
//             std::string contentLen = ss.str();
//             std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + contentLen + "\r\n\r\n" + body;
//             send(fd, response.c_str(), response.size(), 0);
//         }
// }


// void ServerNetwork::handleDelete(int fd, std::string url, Location loc)
// {
//     std::string filePath = loc.root + url;
//     if (remove(filePath.c_str()) == 0)
//     {
//         std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";
//         send(fd, response.c_str(), response.size(), 0);
//     }
//     else
//     {
//         std::string response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
//         send(fd, response.c_str(), response.size(), 0);
//     }
// }

// void ServerNetwork::handlePost(int fd, std::string url, std::string request, Location loc)
// {
//     std::string fileName = url.substr(url.find_last_of("/") + 1);
//     std::string fileCreate = loc.uploadPath + "/" + fileName;
//     std::ofstream outFile(fileCreate.c_str());
//     if (!outFile.is_open())
//     {
//         std::cerr << "Error: could not open file" << std::endl;
//         return ;
//     }
//     outFile << request;
//     std::string response = "HTTP/1.1 201 Created\r\nContent-Length: 0\r\n\r\n";
//     send(fd, response.c_str(), response.size(), 0);
// }
