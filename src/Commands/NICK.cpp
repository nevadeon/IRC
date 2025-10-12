#include "Server.hpp"
#include <set>

static const char disallowedCharsArray[] = { ' ', ',', '*', '?', '!', '@' };
static const int disallowedCharsCount = sizeof(disallowedCharsArray) / sizeof(disallowedCharsArray[0]);
std::set<char> disallowedChars(disallowedCharsArray, disallowedCharsArray + disallowedCharsCount);

static const char disallowedStartCharsArray[] = { '$', ':', '#', '&', '+', '%', '~', '@', '!' };
static const int disallowedStartCharsCount = sizeof(disallowedStartCharsArray) / sizeof(disallowedStartCharsArray[0]);
std::set<char> disallowedStartChars(disallowedStartCharsArray, disallowedStartCharsArray + disallowedStartCharsCount);

bool isValidNickname(const std::string& nickname) {
    if (nickname.empty()) return false;

    for (std::string::size_type i = 0; i < nickname.length(); ++i) {
        if (disallowedChars.find(nickname[i]) != disallowedChars.end()) return false;
    }

    if (disallowedStartChars.find(nickname[0]) != disallowedStartChars.end()) return false;

    return true;
}

/*
    ARGS: [COMMAND = NICK] <nick>
    ERR: 431 432 433

    Check if nickname already used by another user
*/
int Server::Commands::NICK(Server& server, int fd, std::vector<std::string>& args)
{
    std::vector<std::string> params;

    if (!server.clients_.count(fd))
        return (1); // Exception: no fd in clients
    // Client c = server.clients_[fd];
    if (args.size() < 2)
    {
        //431     ERR_NONICKNAMEGIVEN
        // ":No nickname given"
        params.push_back(MSG_NONICKNAMEGIVEN);
        server.Reply(fd, server.info_.servername, std::string("ERR_NONICKNAMEGIVEN"), params);
        return (0); 
    }
    std::string nickname = args[1];
    if (server.IsNicknameAlreadyUsed(nickname)) // Check if valid nickname (case-insensitive)
    {
        // 433     ERR_NICKNAMEINUSE
        // "<nick> :Nickname is already in use"
        params.push_back(nickname);
        params.push_back(MSG_NICKNAMEINUSE);
        server.Reply(fd, server.info_.servername, std::string(ERR_NICKNAMEINUSE), params);
        return (0);
    }
    if (!isValidNickname(nickname)) //Also check length, if begins with letter, no special characters, no whitespace
    {
        //432 ERR_ERRONEUSNICKNAME
        // "<nick> :Erroneus nickname"
        params.push_back(nickname);
        params.push_back(MSG_ERRONEUSNICKNAME);
        server.Reply(fd, server.info_.servername, std::string(ERR_ERRONEUSNICKNAME), params);
        return (0);
    }

    server.clients_[fd].SetNick(nickname);
    // 001 Alice :Welcome to the Internet Relay Network Alice!alice@host
    // 002 Alice :Your host is irc.example.com, running version 2.10
    // 003 Alice :This server was created Thu Oct 09 2025
    // 004 Alice irc.example.com 2.10 ao mtov 
    if (server.clients_[fd].GetUserInfoGiven()) {
        params.push_back(std::string("Welcome to the Internet Relay Network" + nickname + "!" + server.clients_[fd].GetUserInfo().hostname));
        server.Reply(fd, server.info_.servername, nickname, params);
        params.push_back(std::string("Your host is" + server.info_.servername + ", running version " + server.info_.version));
        server.Reply(fd, server.info_.servername, nickname, params);
    }
    
   return (0);
}