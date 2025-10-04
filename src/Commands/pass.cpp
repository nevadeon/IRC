#include "Server.hpp"

int Server::Commands::pass(Server& server, int fd, std::vector<std::string>& args)
{
    std::string password = args[1];
    std::string serv_password = server.GetPassword();

    if (password == serv_password)
    {
        if (server.connected_clients_.count(fd))
        {
            // 434 ERR_ALREADYREGISTERED
        }
        server.unauthenticated_clients_[fd].ValidatePassword();
    }
    else
    {
        // 464 ERR_PASSWDMISMATCH
        server.Disconnect(fd);
    }
}