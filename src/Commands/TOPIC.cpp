#include "Server.hpp"

/*
    Numeric Replies:

    ERR_NEEDMOREPARAMS              ERR_NOTONCHANNEL
    RPL_NOTOPIC                     RPL_TOPIC
    ERR_CHANOPRIVSNEEDED
*/

// ERR_NEEDMOREPARAMS (461)
// Pas assez de paramètres (ex : TOPIC sans nom de channel).

// ERR_NOSUCHCHANNEL (403)
// Le channel n’existe pas.

// ERR_NOTONCHANNEL (442)
// Tu n’es pas membre du channel.

// ERR_CHANOPRIVSNEEDED (482)
// Tu n’es pas opérateur du channel (et le mode +t est actif).

int Server::Commands::TOPIC(Server &server, int fd, std::vector<std::string> &args)
{
    int argsSize = args.size();
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
    std::string chanName = args[1];

    Channel *channel = server.FindChannel(chanName);
    if (!channel) {
        // 403     ERR_NOSUCHCHANNEL
        // "<channel name> :No such channel"
        params.push_back(server.clients_[fd].GetNick());
        params.push_back(chanName);
        params.push_back(MSG_NOSUCHCHANNEL);
        server.Reply(fd, server.info_.servername, std::string(ERR_NOSUCHCHANNEL), params);
        return (0);
    }

    // std::map<Client *, operator_status> clients = channel->GetClients();
    // if (clients.find(client) == clients.end()) {
    if (channel->FindClient(client.GetNick(), server) == -1) {
        // 442     ERR_NOTONCHANNEL
        // "<channel> :You're not on that channel"
        params.push_back(chanName);
        params.push_back(MSG_NOTONCHANNEL);
        server.Reply(fd, server.info_.servername, std::string(ERR_NOTONCHANNEL), params);
        return (0);
    }

    if (argsSize > 2) {
        if (channel->GetModeState('t') && !channel->IsOperator(fd)) {
            // 482     ERR_CHANOPRIVSNEEDED
            // "<channel> :You're not channel operator"
            params.push_back(chanName);
            params.push_back(MSG_CHANOPRIVSNEEDED);
            server.Reply(fd, server.info_.servername, std::string(ERR_CHANOPRIVSNEEDED), params);
            return (0);
        }
        channel->SetTopic(args[2]);
        // :<nick>!<user>@<host> TOPIC test :truc
        std::map<int, operator_status> &clientsMap = channel->GetClients();
        std::string info = client.GetNick() + "!" + client.GetUserInfo().username + "@" + DUMMY_HOSTNAME;
        params.push_back(channel->GetName());
        params.push_back(channel->GetTopic());
        for(std::map<int, operator_status>::iterator it = clientsMap.begin(); it != clientsMap.end(); it++){
            server.Reply(it->first, info, "TOPIC", params);
        }
        return (0);
    }
    params.push_back(client.GetNick());
    params.push_back(chanName);
    params.push_back(channel->GetTopic());
    server.Reply(fd, server.info_.servername, std::string("331"), params);

    return (0);
}