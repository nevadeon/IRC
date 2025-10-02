#include "Commands.hpp"

int Commands::pass(Server& server, int fd, std::vector<std::string>& args)
{
    std::string password = args[1];
    std::string serv_password = server.GetPassword();

    // if (password == serv_password)
    // {
    //     //Also check if already registered
    //     if (server.GetAuthenticatedClients().count(fd))
    //         //ERROR REPLY: ERR_ALREADYREGISTERED
    //     else
    //         server.AuthenticateClient(sender);
    // }
    // else
}