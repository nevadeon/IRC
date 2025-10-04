#include "Server.hpp"

/*
    Format of a server reply:
    [:prefix] (<-osef) code [params] [:trailing]
    Ex: :irc.example.com 001 mynick :Welcome to the IRC Network mynick!~user@host

    -> Up to command to construct the correct code and params
*/
void Server::Reply(int sender_fd, int receiver_fd, const char *code, const char *params, const char *trailing)
{
    return ;
}