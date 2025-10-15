#include "Server.hpp"


/*
    ARGS: [COMMAND = NICK] <nick>
    ERR: 431 432 433

    Check if nickname already used by another user
*/
int Server::Commands::NICK(Server& server, int fd, std::vector<std::string>& args)
{
    std::vector<std::string> params;

    std::string old_nickname = server.clients_[fd].GetNick();
    if (args.size() < 2)
    {
        //431     ERR_NONICKNAMEGIVEN
        // ":No nickname given"
        params.push_back(old_nickname);
        params.push_back(MSG_NONICKNAMEGIVEN);
        server.Reply(fd, server.info_.servername, ERR_NONICKNAMEGIVEN, params);
        return (0); 
    }
    std::string nickname = args[1];
    if (server.IsNicknameAlreadyUsed(nickname)) // Check if valid nickname (case-insensitive)
    {
        // 433     ERR_NICKNAMEINUSE
        // "<nick> :Nickname is already in use"
        params.push_back(old_nickname);
        params.push_back(nickname);
        params.push_back(MSG_NICKNAMEINUSE);
        server.Reply(fd, server.info_.servername, ERR_NICKNAMEINUSE, params);
        return (0);
    }

    if (!Util::isValidNickname(nickname))
    {
        //432 ERR_ERRONEUSNICKNAME
        // "<nick> :Erroneus nickname"
        params.push_back(old_nickname);
        params.push_back(nickname);
        params.push_back(MSG_ERRONEUSNICKNAME);
        server.Reply(fd, server.info_.servername, ERR_ERRONEUSNICKNAME, params);
        return (0);
    }
    
    if (server.clients_[fd].SetNick(nickname))
        server.WelcomeServer(fd);
    else //NotifyAll all registered users of the nickname change
    {
        params.push_back(nickname);
        server.NotifyAll(old_nickname, "NICK", params);
    }

    
   return (0);
}