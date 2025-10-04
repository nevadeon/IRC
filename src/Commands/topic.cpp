#include "Server.hpp"

int Server::Commands::topic(Server& server, int fd, std::vector<std::string>& args)
{
    (void)args; return 0;
}