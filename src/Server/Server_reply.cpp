#include "Server.hpp"

#define AVAILABLE_USER_MODES "o"
#define AVAILABLE_CHANNEL_MODES "itkln"
#define ISUPPORT_TOKEN "CHANTYPES=#"

/*
    Format of a server reply:
    [:prefix] (<-osef) code [params] [:trailing]
    Ex: :irc.example.com 001 mynick :Welcome to the IRC Network mynick!~user@host

    -> Up to command to construct the correct code and params
*/
void Server::Reply(int fd, std::string& prefix, const std::string& code, std::vector<std::string>& params)
{
    std::string str = ":" + prefix + " " + code; //+ params + ":" +  trailing
    for(std::vector<std::string>::iterator it = params.begin(); it != params.end(); it++)
    {
        str += " ";
        // if (*it == params.back() && (it->find(" ") != std::string::npos))
        //     str += ":";
        str += *it;
    }
    str += "\r\n";
    send(fd, str.data(), str.length(), 0);
}

/*
    001 RPL_WELCOME
    002 RPL_YOURHOST
    003 RPL_CREATED
    004 RPL_MYINFO
    005 RPL_ISUPPORT -> CHANTYPE=#
*/
void Server::WelcomeServer(int fd)
{
    std::vector<std::string> params;

    params.push_back(clients_[fd].GetNick());
    params.push_back("Welcome to the " + info_.realname + " Relay Network " + clients_[fd].GetNick());
    Reply(fd, info_.servername, RPL_WELCOME, params);
    params.clear();
    params.push_back(clients_[fd].GetNick());
    params.push_back("Your host is " + info_.servername + ", running version " + info_.version);
    Reply(fd, info_.servername, RPL_YOURHOST, params);
    params.clear();
    params.push_back(clients_[fd].GetNick());
    params.push_back("This server was created a rong rong time ago");
    Reply(fd, info_.servername, RPL_CREATED, params);
    params.clear();
    params.push_back(clients_[fd].GetNick());
    params.push_back(info_.servername);
    params.push_back(info_.version);
    params.push_back(AVAILABLE_USER_MODES);
    params.push_back(AVAILABLE_CHANNEL_MODES);
    Reply(fd, info_.servername, RPL_MYINFO, params);
    params.clear();
    params.push_back(clients_[fd].GetNick());
    params.push_back(ISUPPORT_TOKEN);
    params.push_back("are supported by this server");
    Reply(fd, info_.servername, RPL_ISUPPORT, params);
    params.clear();
    params.push_back(clients_[fd].GetNick());
    params.push_back("get ready to be blackholed");
    Reply(fd, info_.servername, RPL_MOTD, params);
}

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

void Server::WelcomeChannel(int fd, Channel &channel) {
    std::vector<std::string> params;
    std::map<int, operator_status> clientsMap = channel.GetClients();
    Client client = clients_[fd];
    
    // :<nick>!<user>@<host> JOIN <channel>
    std::string info = client.GetNick() + "!" + client.GetUserInfo().username + "@" + DUMMY_HOSTNAME;
    params.push_back(channel.GetName());
    for(std::map<int, operator_status>::iterator it = clientsMap.begin(); it != clientsMap.end(); it++){
        Reply(it->first, info, "JOIN", params);
    }
    params.clear();
    
    // :<servername> 332 <nick> <channel> :<topic>
    params.push_back(client.GetNick());
    params.push_back(channel.GetName());
    params.push_back(channel.GetTopic());
    Reply(fd, info_.servername, RPL_TOPIC, params);
    params.clear();
    

    // :<servername> 353 <nick> = <channel> :@membre1 membre2 ...
    std::string clientsList = "@";
    for(std::map<int, operator_status>::iterator it = clientsMap.begin(); it != clientsMap.end(); it++){
        if (it != clientsMap.begin())
            clientsList = clientsList.append(" ");
        clientsList = clientsList.append((clients_[it->first]).GetNick());
    }
    params.push_back(client.GetNick());
    params.push_back("=");
    params.push_back(channel.GetName());
    params.push_back(clientsList);
    Reply(fd, info_.servername, std::string("353"), params);
    params.clear();
    
    // :<servername> 366 <nick> <channel> :End of /NAMES list.
    params.push_back(client.GetNick());
    params.push_back(channel.GetName());
    params.push_back("End of /NAMES list.");
    Reply(fd, info_.servername, std::string("366"), params);
    params.clear();
    
}

void Server::NotifyAll(std::string& prefix, const std::string& code, std::vector<std::string>& params)
{
    std::map<int, Client>::iterator it = clients_.begin();
    for (; it != clients_.end(); it++)
    {
        if (it->second.IsAuthenticated())
            Reply(it->second.GetFD(), prefix, code, params);
    }
}

void Server::SendToChannel(int fd, Channel& channel, std::string& msg)
{
    std::vector<std::string> params;

    Client sender = clients_[fd];
    std::string info = sender.GetNick() + "!" + sender.GetUserInfo().username + "@" + DUMMY_HOSTNAME;
    params.push_back(channel.GetName());
    params.push_back(msg);
    
    std::map<int, operator_status>::iterator it = channel.GetClients().begin();
    for (; it != channel.GetClients().end(); it++)
    {
        if (fd != it->first)
            Reply(it->first, info, "PRIVMSG", params);
    }
}