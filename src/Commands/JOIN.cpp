#include "Server.hpp"
/*
    JOIN <channel> <key>
    Numeric replies:
    ERR_NEEDMOREPARAMS              ERR_BANNEDFROMCHAN
    ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
    ERR_CHANNELISFULL               ERR_BADCHANMASK
    ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
    RPL_TOPIC

    

*/
int Server::Commands::JOIN(Server& server, int fd, std::vector<std::string>& args)
{
    (void)server;
    (void)fd;
    (void)args;
    return 0;
}