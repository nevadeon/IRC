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
    if (!server.clients_.count(fd))
        return (1); // Exception: no fd in clients
    Client c = server.clients_[fd];
    if (args.size() < 2)
    {
        //431
        return (0); 
    }
    std::string nickname = args[1];
    if (server.IsNicknameAlreadyUsed(nickname)) // Check if valid nickname (case-insensitive)
    {
        //433
        return (0);
    }
    if (!isValidNickname(nickname)) //Also check length, if begins with letter, no special characters, no whitespace
    {
        //432
        return (0);
    }
    
   return (0);
}