#include "ParseUtil.hpp"

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
        throw std::runtime_error("invalid port (not a number)");

    int port = atoi( argv[1] );
    if (port < 1024 || port > 49151)
        throw std::runtime_error("port out of range (1024-49151)");

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