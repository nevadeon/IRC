#include "Server.hpp"

int Server::Commands::part(Server& server, int fd, std::vector<std::string>& args)
{
    (void)args; return 0;
}