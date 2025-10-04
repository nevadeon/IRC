#include "Server.hpp"

/*
    CAP command can be ignored, no need to reply (à tester)
*/
int Server::Commands::cap(Server& server, int fd, std::vector<std::string>& args)
{
    (void)args;
    (void)server;
    (void)fd;
    return 0;
}