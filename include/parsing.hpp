#ifndef PARSING_HPP
#define PARSING_HPP

#include <string>
#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include "colors.hpp"

uint16_t parsePort(char const *argv[]);
std::string getData( std::string type );

bool parseCommandes();


#endif
