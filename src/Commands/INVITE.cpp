#include "Server.hpp"

// INVITE <nick> <channel>

// ERR_NEEDMOREPARAMS (461) : pas assez de paramètres
// ERR_NOSUCHNICK (401) : le pseudo n’existe pas
// ERR_NOTONCHANNEL (442) : tu n’es pas sur le channel
// ERR_CHANOPRIVSNEEDED (482) : tu n’es pas opérateur (si nécessaire)

// ERR_USERONCHANNEL (443) : l’utilisateur est déjà sur le channel
int Server::Commands::INVITE(Server& server, int fd, std::vector<std::string>& args)
{
    std::vector<std::string> params;
    Client client = server.clients_[fd];

    if (args.size() < 3) {
        // 461     ERR_NEEDMOREPARAMS
        // "<command> :Not enough parameters"
        params.push_back("USER");
        params.push_back(MSG_NEEDMOREPARAMS);
        server.Reply(fd, server.info_.servername, std::string(ERR_NEEDMOREPARAMS), params);
        return (0);
    }

    std::string chanName = args[2];
    Channel *channel = server.FindChannel(chanName);
    int clientFd = server.FindClient(args[1]);
    if (clientFd == -1 || !channel) {
        // 401     ERR_NOSUCHNICK
        // "<nickname> :No such nick/channel"
        params.push_back(server.clients_[fd].GetNick());
        params.push_back(MSG_NOSUCHNICK);
        server.Reply(fd, server.info_.servername, std::string(ERR_NOSUCHNICK), params);
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

    if (!channel->IsOperator(fd)) {
        // 482     ERR_CHANOPRIVSNEEDED
        // "<channel> :You're not channel operator"
        params.push_back(chanName);
        params.push_back(MSG_CHANOPRIVSNEEDED);
        server.Reply(fd, server.info_.servername, std::string(ERR_CHANOPRIVSNEEDED), params);
        return (0);
    }
    
    if (channel->FindClient(server.clients_[clientFd].GetNick(), server) != -1) {
        // 443     ERR_USERONCHANNEL
        // "<user> <channel> :is already on channel"
        params.push_back(server.clients_[clientFd].GetNick());
        params.push_back(chanName);
        params.push_back(MSG_USERONCHANNEL);
        server.Reply(fd, server.info_.servername, std::string(ERR_USERONCHANNEL), params);
        return (0);
    }

    channel->Invite(clientFd);

    // :<inviteur>!<user>@<host> INVITE <nick> <channel>
    std::string info = client.GetNick() + "!" + client.GetUserInfo().username + "@" + DUMMY_HOSTNAME;
    params.push_back(client.GetNick());
    params.push_back(chanName);
    server.Reply(clientFd, info, "INVITE", params);
    return (0);
}