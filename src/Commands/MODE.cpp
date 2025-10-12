#include "Server.hpp"

static bool IsAChanOp(Channel* ch, Client *cl)
{
    return (ch->GetClients()[cl] == IS_OPERATOR);
}

static bool IsKnownModeString(std::string& str)
{
    (void)str; return true;
}

/*
    ONLY ACCEPT ONE MODESTIRNG
    Args: MODE <target> []
*/
int Server::Commands::MODE(Server& server, int fd, std::vector<std::string>& args)
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
        // 324 RPL_CHANNELMODEIS
        return (0);
    }

    if (!IsAChanOp(ch, &server.clients_[fd]))
    {
        // ERR_CHANOPRIVSNEEDED (482)
    }

    std::string modes = args[2];
    if (IsKnownModeString(modes))
    {

    }
    return (0);
}