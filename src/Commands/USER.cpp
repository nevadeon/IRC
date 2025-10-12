#include "Server.hpp"


/*
    Parameters: <username> 0 * <realname> -> 5 args including command
    The minimum length of <username> is 1, ie. it MUST NOT be empty.
    !! REALNAME MAY CONTAIN SPACES
*/
int Server::Commands::USER(Server& server, int fd, std::vector<std::string>& args)
{
    std::vector<std::string> params;

    if (args.size() < 5)
    {
        // 461     ERR_NEEDMOREPARAMS
        // "<command> :Not enough parameters"
        params.push_back("USER");
        params.push_back(MSG_NEEDMOREPARAMS);
        server.Reply(fd, server.info_.servername, std::string(ERR_NEEDMOREPARAMS), params);
        return (0);
    }

    if (server.clients_[fd].GetUserInfoGiven())
    {
        // 462     ERR_ALREADYREGISTRED
        // ":You may not reregister"
        params.push_back(MSG_ALREADYREGISTRED);
        server.Reply(fd, server.info_.servername, std::string(ERR_ALREADYREGISTRED), params);
        // ERR_ALREADYREGISTERED
        return (0);
    }

    struct user_info info;
    info.username = args[1];
    if (info.username.size() == 0)
    {
        // 461     ERR_NEEDMOREPARAMS
        // "<command> :Not enough parameters"
        params.push_back("USER");
        params.push_back(MSG_NEEDMOREPARAMS);
        server.Reply(fd, server.info_.servername, std::string(ERR_NEEDMOREPARAMS), params);
        return (1);
    }
    
    info.hostname = args[2];
    info.servername = args[3];
    info.realname = args[4];
    server.clients_[fd].SetUserInfo(info);

    // if client.GetIfNicknameValidated()
    // 001 Alice :Welcome to the Internet Relay Network Alice!alice@host
    // 002 Alice :Your host is irc.example.com, running version 2.10
    // 003 Alice :This server was created Thu Oct 09 2025
    // 004 Alice irc.example.com 2.10 ao mtod
    if (server.clients_[fd].GetIfNicknameValidated()) {
        params.push_back(std::string(server.clients_[fd].GetNick() + " Welcome to the " + server.info_.realname + " Relay Network " + server.clients_[fd].GetNick()));
        server.Reply(fd, server.info_.servername, RPL_WELCOME, params);
        params.clear();
        params.push_back(std::string(server.clients_[fd].GetNick() + "Your host is" + server.info_.realname + ", running version " + server.info_.version));
        server.Reply(fd, server.info_.servername, RPL_YOURHOST, params);
    }

    return (0);
}