#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include "colors.hpp"

class Util
{
    private:
        Util(); //prevent instantiation
    public:
        static bool isNumber(const char *str);
        static uint16_t parsePort(const char *argv[]);
};

static bool isNumber(const char *str)
{
    while (*str)
    {
        if (*str > '9' || *str < '0')
            return ( false );
        str++;
    }
    return ( true );
}

static uint16_t parsePort(char const *argv[])
{
    if (!isNumber( argv[1] ))
        throw std::runtime_error("invalid port (not a number)");

    int port = atoi( argv[1] );
    if (port < 1024 || port > 49151)
        throw std::runtime_error("port out of range (1024-49151)");

    return static_cast<uint16_t>(port);
}

#endif