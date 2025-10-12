#include "Server.hpp"

/*
    ARGS: [COMMAND = PASS] <password>

    Error handling (e.g if user not in unauthenticated map) not done
    Testing required once ParseInput done
*/
int Server::Commands::PASS(Server& server, int fd, std::vector<std::string>& args)
{
    std::vector<std::string> params;
    if (server.clients_[fd].IsAuthenticated())
    {
        // server.Reply(fd, fd, "462", server.connected_clients_[fd].GetNick().c_str(), "You may not reregister"); //CHANGE TO VECTOR
        // 462     ERR_ALREADYREGISTRED
        // ":You may not reregister"
        params.push_back(MSG_ALREADYREGISTRED);
        server.Reply(fd, server.info_.servername, std::string(ERR_ALREADYREGISTRED), params);
        return (0);
    }

    if (args.size() < 2)
    {
        params.push_back(MSG_NEEDMOREPARAMS);
        server.Reply(fd, server.info_.servername, std::string(ERR_NEEDMOREPARAMS), params);
        return (0);
    }
    std::string password = args[1];
    std::string serv_password = server.GetPassword();

    if (password == serv_password)
        server.clients_[fd].ValidatePassword();
    else
    {
        // server.Reply(fd, fd, "464", server.unauthenticated_clients_[fd].GetNick().c_str(), "Password incorrect");
        // 464     ERR_PASSWDMISMATCH
        // ":Password incorrect"
        params.push_back(server.clients_[fd].GetNick());
        params.push_back(MSG_PASSWDMISMATCH);
        server.Reply(fd, server.info_.servername, ERR_PASSWDMISMATCH, params);
    }
    return (0);
}