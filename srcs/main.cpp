
#include "../includes/Parser.hpp"

int main(int ac, char **av)
{
    if (ac != 2)
    {
        std::cerr << "Error: bad argument: ./webserv [configuration file]" << std::endl;
        return (1);
    }

    Parser p(av[1]);
    std::vector<Server> server = p.parse();
    std::vector<Server>::iterator it = server.begin();
    while (it != server.end())
    {
        std::cout << it->port << std::endl;
        std::cout << it->serverName << std::endl;
        std::cout << it->MaxSizeClient << std::endl;
        std::map<int, std::string>::iterator iter = it->errorPage.begin();
        while(iter != it->errorPage.end())
        {
            std::cout << iter->first << " " << iter->second << std::endl;
            iter++;
        }
        std::map<std::string, Location>::iterator itt = it->location.begin();
        while(itt != it->location.end())
        {
            std::cout << itt->second.root << std::endl;
            std::cout << itt->second.fileIndex << std::endl;
            itt++;
        }
        it++;
    }
    return (0);
}
