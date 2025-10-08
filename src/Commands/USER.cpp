#include "Server.hpp"


/*
    Parameters: <username> 0 * <realname> -> 5 args including command
    The minimum length of <username> is 1, ie. it MUST NOT be empty.
    !! REALNAME MAY CONTAIN SPACES
*/
int Server::Commands::USER(Server& server, int fd, std::vector<std::string>& args)
{
    if (args.size() < 5)
    {
        // ERR_NEEDMOREPARAMS
        return (0);
    }

    if (server.clients_[fd].GetUserInfoGiven())
    {
        // ERR_ALREADYREGISTERED
        return (0);
    }

    struct user_info info;
    info.username = args[1];
    if (info.username.size() == 0)
    {
        // ERR_NEEDMOREPARAMS
        return (1);
    }
    
    info.hostname = args[2];
    info.servername = args[3];
    info.realname = args[4];
    server.clients_[fd].SetUserInfo(info);

    return (0);
}