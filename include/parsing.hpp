#ifndef PARSING_HPP
# define PARSING_HPP

// pour les includes je sais que c'est chiant mais c'est mieux de mettre le max
// d'includes possibles dans les fichiers cpp directement ca reduit grandement
// le temps de compilation
# include <string>

// pour eviter tous les bugs lies au couleurs dans le terminal il faut entourer
// les codes ansi de \001 et \002 ca dit au terminal attention le prochain
// caractere est invisible et ne peut pas etre imprime
# define RED "\001\033[31m\002"
# define GREEN "\001\033[1;32m\002"
# define PURPLE "\001\033[1;35m\002"
# define BROWN "\001\033[1;33m\002"
# define RESET "\001\033[0m\002"

int validateArgs(char const *argv[]);
std::string getData( std::string type );

// a mettre autre part ?
bool parseCommandes();


#endif
