#include "Server.hpp"

/*
    ARGS: [COMMAND = NICK] <nick>
    ERR: 431 432 433

    Check if nickname already used by another user
*/
int Server::Server::Commands::nick(Server& server, int fd, std::vector<std::string>& args)
{
    (void)fd;
    if (args.size() < 2) return (1); //431

    for (std::map<int, Client>::iterator it = server.unauthenticated_clients_.begin(); it != server.unauthenticated_clients_.end(); it++)
    {

    }
    for (std::map<int, Client>::iterator it = server.connected_clients_.begin(); it != server.connected_clients_.end(); it++)
    {
        
    }
    return (0);
}