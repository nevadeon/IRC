#include "Server.hpp"

int Server::Commands::USER(Server& server, int fd, std::vector<std::string>& args)
{
    (void)server;
    (void)fd;
    (void)args;
    return 0;
}