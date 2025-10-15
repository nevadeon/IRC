#include "Server.hpp"
#include <set>
#include <sstream>
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
    std::vector<std::string> params;
    Client client = server.clients_[fd];

    if (args.size() < 2)
    {
        // 461     ERR_NEEDMOREPARAMS
        // "<command> :Not enough parameters"
        params.push_back("USER");
        params.push_back(MSG_NEEDMOREPARAMS);
        server.Reply(fd, server.info_.servername, std::string(ERR_NEEDMOREPARAMS), params);
        return (0);
    }

    std::string chanName;
    if (args[1][0] != '#')
        chanName = std::string("#").append(args[1]);
    else
        chanName = args[1];
    Channel *channel = server.FindChannel(chanName);
    if (!channel) {
        // 403     ERR_NOSUCHCHANNEL
        // "<channel name> :No such channel"
        params.push_back(chanName);
        params.push_back(MSG_NOSUCHCHANNEL);
        server.Reply(fd, server.info_.servername, std::string(ERR_NOSUCHCHANNEL), params);
        return (0);
    }

    

    std::cout << "truc 1" << std::endl;
    // ERREUR A CETTE ENDROIT ???
    if (channel->FindClient(client.GetNick(), server) == -1) {
        // 442     ERR_NOTONCHANNEL
        // "<channel> :You're not on that channel"
        params.push_back(chanName);
        params.push_back(MSG_NOTONCHANNEL);
        server.Reply(fd, server.info_.servername, std::string(ERR_NOTONCHANNEL), params);
        return (0);
    }
    std::cout << "truc 2" << std::endl;

    // :blackhole.boys.com 324 Alice #test +kt truc\r\n

    if (args.size() < 3)
    {
        // 324     RPL_CHANNELMODEIS
        // "<channel> <mode> <mode params>"
        const size_t nbMode = 4;
        const char modeChars[nbMode] = { 'i', 't', 'k', 'l' };
        std::string info = "+";
        for (size_t i = 0; i < nbMode; ++i) {
            if (channel->GetModeState(modeChars[i]))
                info = info.append(&modeChars[i]);
        }
        params.push_back(client.GetNick());
        params.push_back(MSG_NOTONCHANNEL)
        if (info.size() > 1)
            params.push_back(info);
        if (channel->GetModeState('k'))
            params.push_back(channel->GetKey());
        if (channel->GetModeState('l')) {
            std::stringstream ss;
            ss << channel->GetUserLimit();
            params.push_back(ss.str());
        }
        std::cout << "truc 2.5" << std::endl;
        server.Reply(fd, server.info_.servername, std::string("324"), params);
        return (0);
    }

    std::cout << "truc 3" << std::endl;


    if (channel->IsOperator(fd) != IS_OPERATOR)
    {
        // 482     ERR_CHANOPRIVSNEEDED 442 M
        // "<channel> :You're not channel operator"
        params.push_back(chanName);
        params.push_back(MSG_CHANOPRIVSNEEDED);
        server.Reply(fd, server.info_.servername, std::string(ERR_CHANOPRIVSNEEDED), params);
        return (0);
    }


    std::string modes = args[2];
    // if (!IsKnownModeString(modes))
    // {
    //      ERR_UNKNOWNMODES
        // 472 — ERR_UNKNOWNMODE

        // Quand : le ou les flags passés ne sont pas des modes de channel connus par le serveur.
        // Exemple : MODE #chan +z (si z n’est pas supporté).
        // Action : renvoyer 472 <mode> "is unknown mode char to me".
    // }



    // :<nick> MODE <channel> <modestring> [params]

    // CHANGER TOPIC EN FONCTION DU MODE !!!!!!!!!!!!
    char sign = '|';
    for (size_t i = 0; i < args[2].size(); i++)
    {
        size_t isValide = 0;
        char mode = args[2][i];
        switch (mode)
        {
            case '+':
                sign = '+';
                break;
            case '-':
                sign = '-';
                break;
            case 'i':
                // IMode();
                if (sign != '+' && sign != '-')
                    isValide = 2;
                else
                    std::cout << "mode i" << std::endl;
                break;
            case 't':
                // TMode();
                if (sign != '+' && sign != '-')
                    isValide = 2;
                else
                    std::cout << "mode t" << std::endl;
                break;
            case 'o':
                // OMode();
                if (sign != '+' && sign != '-')
                    isValide = 2;
                else
                    std::cout << "mode o" << std::endl;
                break;
            case 'k':
                // KMode();
                if (sign != '+' && sign != '-')
                    isValide = 2;
                else
                    std::cout << "mode k" << std::endl;
                break;
            case 'l':
                // LMode();
                if (sign != '+' && sign != '-')
                    isValide = 2;
                else
                    std::cout << "mode l" << std::endl;
                break;
            default:
                isValide = 1;
                break;
        }
        if (isValide > 0) {
                // 472     ERR_UNKNOWNMODE
                // "<char> :is unknown mode char to me"
                params.push_back(&mode);
                params.push_back(MSG_UNKNOWNMODE);
                server.Reply(fd, server.info_.servername, std::string(ERR_UNKNOWNMODE), params);
                if (isValide == 2)
                    return (0);
        }
    }
    

    // char sign = modes[0];
    // (void)sign;
    // char mode = modes[1];
    // // For all modes, check if parameter is given
    // // Then set appropriate field
    // // ex: modes['i'] = true/false
    // switch (mode)
    // {
    //     case 'i':
    //         // IMode();
    //         break;
    //     case 't':
    //         // TMode();
    //         break;
    //     case 'o':
    //         // OMode();
    //         break;
    //     case 'k':
    //         // KMode();
    //         break;
    //     case 'l':
    //         // LMode();
    //         goto default_label;
    //         break;
    //     default:
    //         default_label:
    //         // 472     ERR_UNKNOWNMODE
    //         // "<char> :is unknown mode char to me"
    //         params.push_back(&mode);
    //         params.push_back(MSG_UNKNOWNMODE);
    //         server.Reply(fd, server.info_.servername, std::string(ERR_UNKNOWNMODE), params);
    //         return (0);
    //         // break;
    // }
    return (0);
}

// 401 — ERR_NOSUCHNICK

// Quand : on tente d’appliquer un mode qui requiert un nick (ex +o nick) et le nick n’existe pas sur le réseau / serveur local.
// Exemple : MODE #chan +o utilisateur_inexistant
// Action : renvoyer 401 <nick> "No such nick/channel".

// 441 — ERR_USERNOTINCHANNEL

// Quand : on tente de modifier l’état (ex +o/-o, +v/-v) d’un utilisateur qui n’est pas présent dans le channel.
// Exemple : MODE #chan +o nick (nick n’est pas membre du channel).
// Action : renvoyer 441 <nick> <channel> "They aren't on that channel".