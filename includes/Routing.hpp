
#ifndef ROUTING_HPP
# define ROUTING_HPP

#include "ServerConfig.hpp"
#include <iostream>
#include <string>

class Routing
{
    private:
    
    public:
    Routing();
    ~Routing();
    Routing(const Routing& src);
    Routing& operator=(const Routing& src);
    Location traitement(std::string request, Server server);
};

#endif