#include "Server.hpp"

/*
    ARGS: [COMMAND = PASS] <password>

    Error handling (e.g if user not in unauthenticated map) not done
    Testing required once ParseInput done
*/
int Server::Commands::pass(Server& server, int fd, std::vector<std::string>& args)
{
    std::string password = args[1];
    std::string serv_password = server.GetPassword();

    if (password == serv_password)
    {
        if (server.connected_clients_.count(fd))
        {
            // 462 ERR_ALREADYREGISTERED
            // server.Reply(fd, fd, "462", server.connected_clients_[fd].GetNick().c_str(), "You may not reregister"); //CHANGE TO VECTOR
        }
        server.unauthenticated_clients_[fd].ValidatePassword();
    }
    else
    {
        // 464 ERR_PASSWDMISMATCH
        // server.Reply(fd, fd, "464", server.unauthenticated_clients_[fd].GetNick().c_str(), "Password incorrect");
        server.Disconnect(fd);
    }
    return (0);
}