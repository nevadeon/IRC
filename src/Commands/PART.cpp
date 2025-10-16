#include "Server.hpp"
#include <ParseUtil.hpp>

// ERR_NEEDMOREPARAMS (461) : pas assez de paramètres (pas de nom de channel)
// ERR_NOSUCHCHANNEL (403) : le channel n’existe pas
// ERR_NOTONCHANNEL (442) : l’utilisateur n’est pas membre du channel

// PART <channel>{,<channel>} [<message>]
// sert a quitter un Channel
int Server::Commands::PART(Server& server, int fd, std::vector<std::string>& args)
{
    std::vector<std::string> params;
    Client client = server.clients_[fd];

    if (args.size() < 2)
    {
        // ERR_NEEDMOREPARAMS (461)
        // : paramètres manquants
        params.push_back("JOIN");
        params.push_back(MSG_NEEDMOREPARAMS);
        server.Reply(fd, server.info_.servername, std::string(ERR_NEEDMOREPARAMS), params);
        return (0);
    }

    std::vector<std::string> listChannel;
    listChannel = Util::split(args[1], ",");

    for(std::vector<std::string>::iterator it = listChannel.begin(); it != listChannel.end(); it++) {
        std::string chanName;
        if ((*it)[0] != '#')
            chanName = std::string("#").append(*it);
        else
            chanName = *it;

        Channel *channel = server.FindChannel(chanName);
        if (!channel) {
            // 403     ERR_NOSUCHCHANNEL
            // "<channel name> :No such channel"
            params.push_back(chanName);
            params.push_back(MSG_NOSUCHCHANNEL);
            server.Reply(fd, server.info_.servername, std::string(ERR_NOSUCHCHANNEL), params);
            return (0);
        }

        if (channel->FindClient(client.GetNick(), server) == -1) {
            // 442     ERR_NOTONCHANNEL
            // "<channel> :You're not on that channel"
            params.push_back(chanName);
            params.push_back(MSG_NOTONCHANNEL);
            server.Reply(fd, server.info_.servername, std::string(ERR_NOTONCHANNEL), params);
            return (0);
        }

        std::map<int, operator_status> &clientsMap = channel->GetClients();
        
        std::string info = client.GetNick() + "!" + client.GetUserInfo().username + "@" + DUMMY_HOSTNAME;
        params.push_back(channel->GetName());
        if (args.size() > 2)
            params.push_back(args[2]);
        for(std::map<int, operator_status>::iterator it = clientsMap.begin(); it != clientsMap.end(); it++){
            server.Reply(it->first, info, "PART", params);
        }

        clientsMap.erase(fd);

    }

    return (0);
}