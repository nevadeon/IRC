#include "Server.hpp"

/*
    CAP command can be ignored
*/
int Server::Commands::cap(Server& server, int fd, std::vector<std::string>& args)
{
    (void)args;
    return 0;
}