#include "Server.hpp"

/*
    ARGS: [COMMAND = PASS] <password>

    Error handling (e.g if user not in unauthenticated map) not done
    Testing required once ParseInput done
*/
int Server::Commands::PASS(Server& server, int fd, std::vector<std::string>& args)
{
    std::string password = args[1];
    std::string serv_password = server.GetPassword();
    std::vector<std::string> params;

    if (password == serv_password)
    {
        // if (server.clients_.count(fd))
        if (server.clients_[fd].GetPasswordState())
        {
            // server.Reply(fd, fd, "462", server.connected_clients_[fd].GetNick().c_str(), "You may not reregister"); //CHANGE TO VECTOR
            // 462     ERR_ALREADYREGISTRED
            // ":You may not reregister"
            params.push_back("ERR_ALREADYREGISTRED");
            params.push_back(ERR_ALREADYREGISTRED);
            server.Reply(fd, server.info_.name, std::string("462"), params);
            return (1);
        }
        server.clients_[fd].ValidatePassword();
    }
    else
    {
        // server.Reply(fd, fd, "464", server.unauthenticated_clients_[fd].GetNick().c_str(), "Password incorrect");
        // 464     ERR_PASSWDMISMATCH
        // ":Password incorrect"
        params.push_back("ERR_PASSWDMISMATCH");
        params.push_back(ERR_PASSWDMISMATCH);
        server.Reply(fd, server.info_.name, std::string("464"), params);
        server.Disconnect(fd);
    }
    return (0);
}