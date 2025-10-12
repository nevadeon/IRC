#include "Server.hpp"

/*
    Numeric Replies:

    ERR_NEEDMOREPARAMS              ERR_NOTONCHANNEL
    RPL_NOTOPIC                     RPL_TOPIC
    ERR_CHANOPRIVSNEEDED
*/
int Server::Commands::TOPIC(Server& server, int fd, std::vector<std::string>& args)
{
    if (args.size() < 2)
    {
        // ERR_NEEDMOREPARAMS
        return (0);
    }

    Channel *ch = server.FindChanel(args[1]);
    if (!ch)
    {
        // 403: No such channel
        return (0);
    }

    if (args.size() < 3)
    {
        // View Topic...
    }

    std::string new_topic = args[2];
    Client cl = server.clients_[fd];
    if (ch->GetModeState()['o'] && !ch->IsOperator(&cl)) // If you need to be operator to change topic
    {
        // ERR_CHANOPRIVSNEEDED (482)
    }

    //Change topic....
}