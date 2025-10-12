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

    if (args.size() < 2)
    {
        //431     ERR_NONICKNAMEGIVEN
        // ":No nickname given"
        params.push_back(MSG_NONICKNAMEGIVEN);
        server.Reply(fd, server.info_.servername, ERR_NONICKNAMEGIVEN, params);
        return (0); 
    }
    std::string nickname = args[1];
    if (server.IsNicknameAlreadyUsed(nickname)) // Check if valid nickname (case-insensitive)
    {
        // 433     ERR_NICKNAMEINUSE
        // "<nick> :Nickname is already in use"
        params.push_back(nickname);
        params.push_back(MSG_NICKNAMEINUSE);
        server.Reply(fd, server.info_.servername, ERR_NICKNAMEINUSE, params);
        return (0);
    }
    if (args.size() > 2 && !isValidNickname(nickname))
    {
        //432 ERR_ERRONEUSNICKNAME
        // "<nick> :Erroneus nickname"
        params.push_back(nickname);
        params.push_back(MSG_ERRONEUSNICKNAME);
        server.Reply(fd, server.info_.servername, ERR_ERRONEUSNICKNAME, params);
        return (0);
    }
    
    std::string old_nickname = server.clients_[fd].GetNick();
    if (server.clients_[fd].SetNick(nickname))
        server.Welcome(fd);
    else //Notify all registered users of the nickname change
    {
        params.push_back(nickname);
        server.Notify(old_nickname, "NICK", params);
    }

    
   return (0);
}