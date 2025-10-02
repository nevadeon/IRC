#include "Server.hpp"

std::string& Server::GetPassword() { return password_; }

/*
    Move Client from unauthenticated map to authenticated one
*/
void Server::AuthenticateClient(Client& client)
{
    int fd = client.GetFD();
    std::string nick = client.GetNick();
    unauthenticated_clients_.erase(fd);
    connected_clients_[fd] = client;
}