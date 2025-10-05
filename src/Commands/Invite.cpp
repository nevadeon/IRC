#include "Server.hpp"

int Server::Commands::INVITE(Server& server, int fd, std::vector<std::string>& args)
{
    (void)server;
    (void)fd;
    (void)args;
    return 0;
}