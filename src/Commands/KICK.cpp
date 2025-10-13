#include "Server.hpp"

int Server::Commands::KICK(Server& server, int fd, std::vector<std::string>& args)
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

    // std::string chanName = args[1];
    std::string chanName;
    if (args[1][0] != '#')
        chanName = std::string("#").append(args[1]);
    else
        chanName = args[1];
    std::string target_nick = args[2];
    std::string reason;
    if (args.size() > 3)
        reason = args[3];
    else
        reason = client.GetNick();
    Channel* channel = server.FindChannel(chanName);
    if (!channel) {
        // 403     ERR_NOSUCHCHANNEL
        // "<channel name> :No such channel"
        params.push_back(chanName);
        params.push_back(MSG_NOSUCHCHANNEL);
        server.Reply(fd, server.info_.servername, std::string(ERR_NOSUCHCHANNEL), params);
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

    int targetFd = server.FindClient(target_nick);
    if (targetFd != -1 || channel->GetClients().find(targetFd) == channel->GetClients().end()) {
        // 441     ERR_USERNOTINCHANNEL
        // "<nick> <channel> :They aren't on that channel"
        params.push_back(client.GetNick());
        params.push_back(chanName);
        params.push_back(MSG_USERNOTINCHANNEL);
        server.Reply(fd, server.info_.servername, std::string(ERR_USERNOTINCHANNEL), params);
        return (0);
    }

    std::string info = client.GetNick() + "!" + client.GetUserInfo().username + "@" + DUMMY_HOSTNAME;
    std::map<int, operator_status> clientsMap = channel->GetClients();
    params.push_back(channel->GetName());
    params.push_back(target_nick);
    params.push_back(reason);
    for(std::map<int, operator_status>::iterator it = clientsMap.begin(); it != clientsMap.end(); it++){
        server.Reply(it->first, info, "KICK", params);
    }
    clientsMap.erase(fd);

    // for(std::map<std::string, Channel>::iterator it = server.channels_.begin(); it != server.channels_.end(); it++)
    //     it->second.GetClients().erase(fd);

    return 0;
}