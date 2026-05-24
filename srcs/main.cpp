
#include "../includes/Parser.hpp"
#include "../includes/ServerNetwork.hpp"

int main(int ac, char **av)
{
    if (ac != 2)
    {
        std::cerr << "Error: bad argument: ./webserv [configuration file]" << std::endl;
        return (1);
    }

    Parser p(av[1]);
    std::vector<Server> server = p.parse();
    if (server.empty())
    {
        std::cerr << "Error: not valid server configuration found" << std::endl;
        return (1);
    }
    
    ServerNetwork network(server);
    network.run();

    return (0);
}
