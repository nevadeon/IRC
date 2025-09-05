#ifndef PARSING_HPP
# define PARSING_HPP

# include <iostream>
# include <cstdlib>
// pour find :
# include <algorithm> // autorise dans le sujet ?
# include <vector>
# include <string>

int validateArgs(char const *argv[]);
std::string getData( std::string type );

// a mettre autre part ?
bool parseCommandes();


#endif
