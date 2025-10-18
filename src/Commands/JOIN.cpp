#include "Server.hpp"
#include "ParseUtil.hpp"

int Server::Commands::JOIN(Server& server, int fd, std::vector<std::string>& args)
{
    std::vector<std::string> params;
    Client client = server.clients_[fd];

    if (args.size() < 2)
    {
        // ERR_NEEDMOREPARAMS (461)
        // : paramètres manquants
        params.push_back("JOIN");
        params.push_back(MSG_NEEDMOREPARAMS);
        server.Reply(fd, server.info_.servername, ERR_NEEDMOREPARAMS, params);
        return (0);
    }

    if (!Util::isValidChannelName(args[1])) {
        // ERR_BADCHANMASK 476
        params.push_back(client.GetNick());
        params.push_back(args[1]);
        params.push_back(MSG_BADCHANMASK);
        server.Reply(fd, server.info_.servername, ERR_BADCHANMASK, params);
        return (0);
    }

    std::vector<std::string> listChannel = Util::split(args[1], ",");
    std::vector<std::string> listMDP;
    std::vector<std::string>::iterator itMDP;

    if (args.size() > 2) {
        listMDP = Util::split(args[2], ",");
        itMDP = listMDP.begin();
    }

    for(std::vector<std::string>::iterator it = listChannel.begin(); it != listChannel.end(); it++)
    {
        std::string chanName = *it;

        Channel *channel = server.FindChannel(chanName);
        if (channel) {
            if (channel->GetModeState('i') && !channel->IsInvitedClient(fd)) {
                // 473     ERR_INVITEONLYCHAN
                // "<channel> :Cannot join channel (+i)"
                params.push_back(client.GetNick());
                params.push_back(channel->GetName());
                params.push_back(MSG_INVITEONLYCHAN);
                server.Reply(fd, server.info_.servername, ERR_INVITEONLYCHAN, params);
                continue;
            }

            if (channel->GetModeState('k')) {
                if (args.size() < 3 || (args.size() > 2 && (itMDP != listMDP.end()) && (channel->GetKey() != *itMDP))
                        || (args.size() > 2 && (itMDP == listMDP.end()))) {
                    // 475     ERR_BADCHANNELKEY
                    // "<channel> :Cannot join channel (+k)"
                    params.push_back(client.GetNick());
                    params.push_back(channel->GetName());
                    params.push_back(MSG_BADCHANNELKEY);
                    server.Reply(fd, server.info_.servername, ERR_BADCHANNELKEY, params);
                    itMDP++;
                    continue;
                }
                itMDP++;
            }

            std::cout << "mode l :" << channel->GetModeState('l') << " " << channel->GetUserLimit() << " " << channel->GetClients().size() << std::endl;

            if (channel->GetModeState('l') && (channel->GetUserLimit() <= channel->GetClients().size())) {
                // 471     ERR_CHANNELISFULL
                // "<channel> :Cannot join channel (+l)"
                params.push_back(client.GetNick());
                params.push_back(channel->GetName());
                params.push_back(MSG_CHANNELISFULL);
                server.Reply(fd, server.info_.servername, ERR_CHANNELISFULL, params);
                continue;
            }

            channel->AddClient(fd);
            server.WelcomeChannel(fd, *channel);
            continue;
        }

        Channel newChan = Channel(chanName, fd);
        newChan.AddClient(BOT_FD);
        server.channels_.insert(std::make_pair(newChan.GetName(), newChan));
        channel->EraseInvitation(fd);
        server.WelcomeChannel(fd, newChan);
    }
    return (0);
}
