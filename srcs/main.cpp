
#include "../includes/Parser.hpp"
#include "../includes/ServerNetwork.hpp"
#include <csignal>

ServerNetwork *g_network = NULL;

void signalHandler(int sig)
{
    (void)sig;
    if (g_network)
        g_network->stop();
    ServerNetwork::_running = false;
}

int main(int ac, char **av)
{
    if (ac != 2)
    {
        std::cerr << "Error: bad argument: ./webserv [configuration file]" << std::endl;
        return (1);
    }

    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    Parser p(av[1]);
    std::vector<Server> server = p.parse();
    if (server.empty())
    {
        std::cerr << "Error: not valid server configuration found" << std::endl;
        return (1);
    }
    
    ServerNetwork network(server);
    g_network = &network;
    network.run();

    return (0);
}
