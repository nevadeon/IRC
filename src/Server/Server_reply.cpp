#include "Server.hpp"

#define AVAILABLE_USER_MODES "o"
#define AVAILABLE_CHANNEL_MODES "itkl"
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
        if (*it == params.back() && (it->find(" ") != std::string::npos))
            str += ":";
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
void Server::Welcome(int fd)
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

void Server::Notify(std::string& prefix, const std::string& code, std::vector<std::string>& params)
{
    std::map<int, Client>::iterator it = clients_.begin();
    for (; it != clients_.end(); it++)
    {
        if (it->second.IsAuthenticated())
            Reply(it->second.GetFD(), prefix, code, params);
    }
}