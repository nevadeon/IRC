#include "Server.hpp"
#include <cstdio>

std::string& Server::GetPassword() { return info_.password; }

/*
    Move Client from unauthenticated map to authenticated one
*/
void Server::AuthenticateClient(int fd)
{
    if (!clients_.count(fd)) throw std::runtime_error("Invalid fd");
    clients_[fd].Authenticate();
}

/*
    Check if nickname already used by connected user
*/
bool Server::IsNicknameAlreadyUsed(std::string& nickname)
{
    
    for (std::map<int, Client>::iterator it = clients_.begin(); it != clients_.end(); it++)
        if (it->second.GetNick() == nickname) return true;
    return false;
}

int Server::FindClient(std::string nickname) {
    for(std::map<int, Client>::iterator it = clients_.begin(); it != clients_.end(); it++) {
        if (it->second.GetNick() == nickname) {
            return (it->first);
        }
    }
    return (-1);
}
