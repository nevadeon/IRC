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

// JOIN <channel>{,<channel>} [<key>{,<key>}]
// ERR_NEEDMOREPARAMS (461) : paramètres manquants

// ERR_BADCHANMASK (476) : nom de channel invalide

// ERR_BANNEDFROMCHAN (474) : banni du channel
// ERR_INVITEONLYCHAN (473) : channel sur invitation seulement
// ERR_BADCHANNELKEY (475) : mauvaise clé
// ERR_CHANNELISFULL (471) : channel plein
// ERR_NOSUCHCHANNEL (403) : channel inexistant
// ERR_TOOMANYCHANNELS (405) : trop de channels rejoints

// Si le JOIN réussit :
// Le client reçoit le topic du channel (RPL_TOPIC).
// Le client reçoit la liste des utilisateurs du channel (RPL_NAMREPLY puis RPL_ENDOFNAMES).

// Exemples :
// JOIN #foobar
// JOIN #foo,#bar key1,key2
// JOIN &local
// JOIN #canal1,#canal2 motdepasse1,motdepasse2

static bool isValidChanelName(std::string chanName) {
    if (chanName[0] != '&' && chanName[0] != '#')
        return (false);
    
    return (true);
}

int Server::Commands::JOIN(Server& server, int fd, std::vector<std::string>& args)
{
    (void)server;
    (void)fd;
    (void)args;
    std::vector<std::string> params;

    if (args.size() < 2)
    {
        // ERR_NEEDMOREPARAMS (461)
        // : paramètres manquants
        params.push_back("JOIN");
        params.push_back(MSG_NEEDMOREPARAMS);
        server.Reply(fd, server.info_.servername, std::string(ERR_NEEDMOREPARAMS), params);
        return (0);
    }


    //  Un channel doit commencer par # ou &
    //  accepte ces caracteres : (ASCII < 32 et 127)
    //  Doit avoir au moins un caractère après le préfixe
    if (!isValidChanelName(args[1])) {
        // ERR_BADCHANMASK 476

        params.push_back(server.clients_[fd].GetNick());
        params.push_back(args[1]);
        params.push_back(MSG_BADCHANMASK);
        server.Reply(fd, server.info_.servername, std::string(ERR_BADCHANMASK), params);
        return (0);
    }
    return (0);
}