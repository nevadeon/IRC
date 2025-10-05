#include "Server.hpp"

/*
    ARGS: [COMMAND = NICK] <nick>
    ERR: 431 432 433

    Check if nickname already used by another user
*/
int Server::Commands::NICK(Server& server, int fd, std::vector<std::string>& args)
{
    if (!server.clients_.count(fd)) return (1);
    Client c = server.clients_[fd];
    if (args.size() < 2)
    {

        return (0); //431
    }
    std::string nickname = args[1];
    if (server.IsNicknameAlreadyUsed(nickname)) // Check if valid nickname (case-insensitive)
    {
        //432
        return (1);
    }
    //Also check length, if begins with letter, no special characters, no whitespace

    if (server.clients_.count(fd))
    {

    }
    else // Person is registered, check and set
    {

    }
    return (0);
}