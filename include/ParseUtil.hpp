#ifndef UTIL_HPP
#define UTIL_HPP

#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

/*
    Useful for organizing utility functions
    without having to instantiate anything.
*/
class Util
{
    private:
        Util(); //prevent instantiation
    public:
        static bool isNumber(const char *str);
        static uint16_t parsePort(const char *argv[]);
        static std::vector<std::string> split(std::string& s, const std::string& delimiter);
};

#endif