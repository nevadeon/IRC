#include "Server.hpp"

/*
    Numeric Replies:

    ERR_NEEDMOREPARAMS              ERR_NOTONCHANNEL
    RPL_NOTOPIC                     RPL_TOPIC
    ERR_CHANOPRIVSNEEDED
*/
int Server::Commands::TOPIC(Server& server, int fd, std::vector<std::string>& args)
{
    (void)server;
    (void)fd;
    (void)args;
    return 0;
}