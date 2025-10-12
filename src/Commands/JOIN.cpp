#include "Server.hpp"
#include "ParseUtil.hpp"
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
// ERR_INVITEONLYCHAN (473) : channel sur invitation seulement
// ERR_BADCHANNELKEY (475) : mauvaise clé

// ERR_CHANNELISFULL (471) : channel plein


// Si le JOIN réussit :
// Le client reçoit le topic du channel (RPL_TOPIC).
// Le client reçoit la liste des utilisateurs du channel (RPL_NAMREPLY puis RPL_ENDOFNAMES).

// Exemples :
// JOIN #foobar
// JOIN #foo,#bar key1,key2
// JOIN &local
// JOIN #canal1,#canal2 motdepasse1,motdepasse2

void Server::WelcomeChanel(Server server, int fd, Channel *channel, Client *client) {
    std::vector<std::string> params;
    std::map<Client *, operator_status> clientsMap = channel->GetClients();
    
    for(std::map<Client *, operator_status>::iterator it = clientsMap.begin(); it != clientsMap.end(); it++){
        // a change
        std::string info = client->GetNick() + "!" + client->GetUserInfo().username + "@" + DUMMY_HOSTNAME;
        server.Reply(it->first->GetFD(), info, channel->GetName(), params);
    }
    params.clear();
    
    // :<servername> 332 <nick> <channel> :<topic>
    std::cout << channel->GetName() << std::endl;
    params.push_back(client->GetNick());
    params.push_back(channel->GetName());
    params.push_back(channel->GetTopic());
    server.Reply(fd, server.info_.servername, std::string("332"), params);
    params.clear();
    

    // :<servername> 353 <nick> = <channel> :@membre1 membre2 ...
    std::string clientsList = "@";
    for(std::map<Client *, operator_status>::iterator it = clientsMap.begin(); it != clientsMap.end(); it++){
        if (it != clientsMap.begin())
            clientsList = clientsList.append(" ");
        clientsList = clientsList.append(it->first->GetNick());
    }
    params.push_back(client->GetNick());
    params.push_back("=");
    params.push_back(channel->GetName());
    params.push_back(clientsList);
    server.Reply(fd, server.info_.servername, std::string("353"), params);
    params.clear();
    
    // :<servername> 366 <nick> <channel> :End of /NAMES list.
    params.push_back(client->GetNick());
    params.push_back(channel->GetName());
    params.push_back("End of /NAMES list.");
    server.Reply(fd, server.info_.servername, std::string("366"), params);
    params.clear();
    
}

static bool isValidChanelName(std::string chanName) {
    size_t lenChanName = chanName.size();
    if (chanName[0] != '#')
        chanName = std::string("#").append(chanName);
    if (lenChanName < 2)
        return (false);
    for (size_t i = 0; i < lenChanName; i++)
        if (chanName[i] > 127 || chanName[i] < 32)
            return (false);
    return (true);
}

int Server::Commands::JOIN(Server& server, int fd, std::vector<std::string>& args)
{
    std::vector<std::string> params;
    Client *client = &(server.clients_[fd]);

    if (args.size() < 2)
    {
        // ERR_NEEDMOREPARAMS (461)
        // : paramètres manquants
        params.push_back("JOIN");
        params.push_back(MSG_NEEDMOREPARAMS);
        server.Reply(fd, server.info_.servername, std::string(ERR_NEEDMOREPARAMS), params);
        return (0);
    }

    if (!isValidChanelName(args[1])) {
        // ERR_BADCHANMASK 476

        params.push_back(client->GetNick());
        params.push_back(args[1]);
        params.push_back(MSG_BADCHANMASK);
        server.Reply(fd, server.info_.servername, std::string(ERR_BADCHANMASK), params);
        return (0);
    }

    std::vector<std::string> listChanel;
    std::vector<std::string> listMDP;
    std::vector<std::string>::iterator itMDP;

    listChanel = Util::split(args[1], ",");
    if (args.size() > 2) {
        listMDP = Util::split(args[2], ",");
        itMDP = listChanel.begin();
    }

    for(std::vector<std::string>::iterator it = listChanel.begin(); it != listChanel.end(); it++)
    {
        Channel *channel = server.FindChanel(*it);
        if (channel) {
            if (channel->GetModeState('i') && !channel->IsInvitedClient(client)) {
                // 473     ERR_INVITEONLYCHAN
                // "<channel> :Cannot join channel (+i)"
                params.push_back(client->GetNick());
                params.push_back(channel->GetName());
                params.push_back(MSG_INVITEONLYCHAN);
                server.Reply(fd, server.info_.servername, std::string(ERR_INVITEONLYCHAN), params);
                continue;
            }

            if (channel->GetModeState('k')) {
                if ((args.size() > 2 && (itMDP != listMDP.end()) && (channel->GetKey() == *itMDP))
                        || (args.size() > 2 && (itMDP == listMDP.end()))) {
                    // 475     ERR_BADCHANNELKEY
                    // "<channel> :Cannot join channel (+k)"
                    params.push_back(client->GetNick());
                    params.push_back(channel->GetName());
                    params.push_back(MSG_BADCHANNELKEY);
                    server.Reply(fd, server.info_.servername, std::string(ERR_BADCHANNELKEY), params);
                    itMDP++;
                    continue;
                }
                itMDP++;
            }
            

            if (channel->GetModeState('l') && (channel->GetUserLimit() < channel->GetClients().size())) {
                // 471     ERR_CHANNELISFULL
                // "<channel> :Cannot join channel (+l)"
                params.push_back(client->GetNick());
                params.push_back(channel->GetName());
                params.push_back(MSG_CHANNELISFULL);
                server.Reply(fd, server.info_.servername, std::string(ERR_CHANNELISFULL), params);
                continue;
            }

            channel->AddClient(client);
            WelcomeChanel(server, fd, channel, client);
            continue;
        }

        Channel newChan = Channel(*it, client);
        WelcomeChanel(server, fd, &newChan, client);
        server.channels_.insert(std::make_pair(newChan.GetName(), newChan));
        
        
    }
    return (0);
    


    
}