#include "Server.hpp"
/*
    ! REMOVE CLIENT FROM ALL CHANNELS WHEN LEAVING THE SERVER

    JOIN <channel> [<key>]
    Numeric replies:
    ERR_NEEDMOREPARAMS              ERR_BANNEDFROMCHAN
    ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
    ERR_CHANNELISFULL               ERR_BADCHANMASK
    ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
    RPL_TOPIC

    Check if channel exists
    -> If not
        -> Create a channel with the corresponding client as founder
    -> If exists,
        -> If +k, 
*/
int Server::Commands::JOIN(Server& server, int fd, std::vector<std::string>& args)
{
    (void)server;
    (void)fd;
    (void)args;

    if (args.size() < 2)
    {

    }
    return (0);
}