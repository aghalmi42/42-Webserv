
int main(int ac, char **av)
{
    if (ac != 2)
    {
        std::cerr << "Error: bad argument: ./webserv [configuration file]" << std::endl;
        return (1);
    }
    return (0);
}