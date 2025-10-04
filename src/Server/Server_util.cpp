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