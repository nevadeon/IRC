#include "Server.hpp"

std::string& Server::GetPassword() { return info_.password; }

/*
    Move Client from unauthenticated map to authenticated one
*/
void Server::AuthenticateClient(int fd)
{
    Client& cl = unauthenticated_clients_[fd];
    unauthenticated_clients_.erase(fd);
    connected_clients_[fd] = cl;
}

/*
    Check if nickname already used by connected user
*/
bool Server::IsNicknameAlreadyUsed(std::string& nickname)
{
    for (std::map<int, Client>::iterator it = unauthenticated_clients_.begin(); it != unauthenticated_clients_.end(); it++)
        if (it->second.GetNick() == nickname) return true;
    for (std::map<int, Client>::iterator it = connected_clients_.begin(); it != connected_clients_.end(); it++)
        if (it->second.GetNick() == nickname) return true;
    return false;
}