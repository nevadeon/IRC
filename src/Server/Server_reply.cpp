#include "Server.hpp"

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

void Server::Welcome(int fd)
{
    std::vector<std::string> params;

    params.push_back(std::string(clients_[fd].GetNick() + " Welcome to the " + info_.realname + " Relay Network " + clients_[fd].GetNick()));
    Reply(fd, info_.servername, RPL_WELCOME, params);
    params.clear();
    params.push_back(std::string(clients_[fd].GetNick() + "Your host is" + info_.realname + ", running version " + info_.version));
    Reply(fd, info_.servername, RPL_YOURHOST, params);
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