#include "ParseUtil.hpp"
#include <set>

static const char disallowedCharsArray[] = { ' ', ',', '*', '?', '!', '@', '.' };
static const int disallowedCharsCount = sizeof(disallowedCharsArray) / sizeof(disallowedCharsArray[0]);
std::set<char> disallowedChars(disallowedCharsArray, disallowedCharsArray + disallowedCharsCount);

static const char disallowedStartCharsArray[] = { '$', ':', '#', '&', '+', '%', '~', '@', '!' };
static const int disallowedStartCharsCount = sizeof(disallowedStartCharsArray) / sizeof(disallowedStartCharsArray[0]);
std::set<char> disallowedStartChars(disallowedStartCharsArray, disallowedStartCharsArray + disallowedStartCharsCount);

bool Util::isNumber(const char *str)
{
    while (*str)
    {
        if (*str > '9' || *str < '0')
            return ( false );
        str++;
    }
    return ( true );
}

uint16_t Util::parsePort(char const *argv[])
{
    if (!isNumber( argv[1] ))
        throw std::runtime_error("Invalid port (not a number)");

    int port = atoi( argv[1] );
    if (port < 1024 || port > 49151)
        throw std::runtime_error("Port out of range (1024-49151)");

    return static_cast<uint16_t>(port);
}

std::vector<std::string> Util::split(std::string& s, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        if (token.length() != 0)
            tokens.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    if (s.length() != 0)
        tokens.push_back(s);

    return tokens;
}

std::string Util::parsePassword(const char *password)
{
    std::string str(password);
    if (str.size() == 0)
        throw std::runtime_error("Password is empty");
    for (size_t i = 0; i < str.size(); i++)
    {
        if (isspace(str[i]))
            throw std::runtime_error("Invalid password format");
    }
    return (str);
}

std::vector<std::string> Util::parseCommand(std::string str)
{
    std::vector<std::string> listArg;
    size_t pos = 0;
    std::string token;
    std::string delimiter = " ";
    while ((pos = str.find(delimiter)) != std::string::npos) {
        if (str[0] == ':') {
            str.erase(0, 1);
            break;
        }
        token = str.substr(0, pos);
        if (token.length() != 0)
            listArg.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    if (str.length() != 0)
        listArg.push_back(str);

    return listArg;
}

bool Util::isValidChannelName(std::string &chanName) {
    if (chanName[0] != '#')
    {
        std::cout << "caca\n";
        return false;
    }

    size_t lenChanName = chanName.size();
    unsigned char c;
    if (lenChanName < 2)
        return (false);
    for (size_t i = 0; i < lenChanName; i++) {
        c = static_cast<unsigned char>(chanName[i]);
        if (c > 127 || c < 32)
        return (false);
    }
    return (true);
}

bool Util::isValidNickname(const std::string& nickname) {
    if (nickname.empty()) return false;

    for (std::string::size_type i = 0; i < nickname.length(); ++i) {
        if (disallowedChars.find(nickname[i]) != disallowedChars.end()) return false;
    }

    if (disallowedStartChars.find(nickname[0]) != disallowedStartChars.end()) return false;

    return true;
}
