
#include "../../includes/Routing.hpp"

Routing::Routing()
{
}

Routing::~Routing()
{
}

Routing::Routing(const Routing& src)
{
    *this = src;
}

Routing& Routing::operator=(const Routing& src)
{
    (void)src;
    return (*this);
}

Location Routing::traitement(std::string request, Server server)
{
    size_t first = request.find(" ");
    size_t begin = first + 1;
    size_t end = request.find(" ", begin);
    std::string url = request.substr(begin, end - begin);
    std::map<std::string, Location>::iterator it = server.location.begin();
    while(it != server.location.end())
    {
        if (it->first == url)
            return (it->second);
        it++;
    }
    return (server.location["/"]);
}
