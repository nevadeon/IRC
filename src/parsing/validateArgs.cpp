#include "parsing.hpp"

static bool isNumber( const char *str )
{
    while (*str)
    {
        if (*str > '9' || *str < '0')
            return ( false );
        str++;
    }
    return ( true );
}

// static bool isAlNum( const std::string& str )
// {
//     int i = 0;
//     while (str[i])
//     {
//         if ((str[i] > '9' || str[i] < '0')
//             && (str[i] > 'z' || str[i] < 'a')
//             && (str[i] > 'Z' || str[i] < 'A'))
//             return ( false );
//         i++;
//     }
//     return ( true );
// }

uint16_t parsePort(char const *argv[])
{
    if (!isNumber( argv[1] ))
        throw std::runtime_error("invalid port (not a number)");

    int port = atoi( argv[1] );
    if (port < 1024 || port > 49151)
        throw std::runtime_error("port out of range (1024-49151)");

    return static_cast<uint16_t>(port);
}
