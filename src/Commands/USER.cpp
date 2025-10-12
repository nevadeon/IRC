#include "Server.hpp"


/*
    Parameters: <username> 0 * <realname> -> 5 args including command
    The minimum length of <username> is 1, ie. it MUST NOT be empty.
    !! REALNAME MAY CONTAIN SPACES
*/
int Server::Commands::USER(Server& server, int fd, std::vector<std::string>& args)
{
    std::vector<std::string> params;

    if (args.size() < 5)
    {
        // 461     ERR_NEEDMOREPARAMS
        // "<command> :Not enough parameters"
        params.push_back("USER");
        params.push_back(MSG_NEEDMOREPARAMS);
        server.Reply(fd, server.info_.servername, std::string(ERR_NEEDMOREPARAMS), params);
        return (0);
    }

    if (server.clients_[fd].GetUserInfoGiven())
    {
        // 462     ERR_ALREADYREGISTRED
        // ":You may not reregister"
        params.push_back(MSG_ALREADYREGISTRED);
        server.Reply(fd, server.info_.servername, std::string(ERR_ALREADYREGISTRED), params);
        return (0);
    }

    struct user_info info;
    info.username = args[1];
    if (info.username.size() == 0)
    {
        // 461     ERR_NEEDMOREPARAMS
        // "<command> :Not enough parameters"
        params.push_back("USER");
        params.push_back(MSG_NEEDMOREPARAMS);
        server.Reply(fd, server.info_.servername, std::string(ERR_NEEDMOREPARAMS), params);
        return (1);
    }
    
    info.hostname = args[2];
    info.servername = args[3];
    info.realname = args[4];

    if (server.clients_[fd].SetUserInfo(info))
        server.Welcome(fd);
    return (0);
}