#include "Server.hpp"
#include <set>

// static bool IsAChanOp(Channel* ch, int fd)
// {
//     return (ch->GetClients()[fd] == IS_OPERATOR);
// }

/*
    (+ ou -).(i, o, t, k, l)
*/
// static bool IsKnownModeString(std::string& str)
// {
//     return (str.size() == 2
//         && (str[0] == '+' || str[0] == '-')
//         && (allowed_modes.count(str[1])));
// }




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

    Channel *ch = server.FindChannel(args[1]);
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

    if (!ch->IsOperator(fd))
    {
        // ERR_CHANOPRIVSNEEDED (482)
    }

    std::string modes = args[2];
    // if (!IsKnownModeString(modes))
    // {
    //     // ERR_UNKNOWNMODES
    // }

    char sign = modes[0];
    (void)sign;
    char mode = modes[1];
    // For all modes, check if parameter is given
    // Then set appropriate field
    // ex: modes['i'] = true/false
    switch (mode)
    {
        // case 'i':
        //     IMode();
        //     break;
        // case 't':
        //     TMode();
        //     break;
        // case 'o':
        //     OMode();
        //     break;
        // case 'k':
        //     KMode();
        //     break;
        // case 'l':
        //     LMode();
        //     break;
        default:
            break;
    }
    return (0);
}