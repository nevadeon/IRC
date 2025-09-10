#ifndef PARSING_HPP
#define PARSING_HPP

// pour les includes je sais que c'est chiant mais c'est mieux de mettre le max
// d'includes possibles dans les fichiers cpp directement ca reduit grandement
// le temps de compilation
#include <string>
#include <stdint.h>
#include "colors.hpp"

uint16_t validateArgs(char const *argv[]);
std::string getData( std::string type );

// a mettre autre part ?
bool parseCommandes();


#endif
