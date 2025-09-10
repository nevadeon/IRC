#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
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

static bool isInTab( std::vector<int>& tabInt, int number )
{
    return ( std::find( tabInt.begin(), tabInt.end(), number ) != tabInt.end() );
}

// voir les ports deja utilises :
// ss -tuln

// mettre un system qui empeche d'utiliser un port deja utiliser par 42 ?
// -> l'appel a la fonction bind renvoie une erreur quand le port est deja utilise
// try {
//     acceptor.bind(endpoint);
// } catch (const boost::system::system_error& e) {
//     std::cerr << "Error: " << e.what() << '\n';
// }
uint16_t validateArgs(char const *argv[])
{
    if (!isNumber( argv[1] ))
        throw std::runtime_error("invalid port (not a number)");

    int port = atoi( argv[1] );
    if (port < 1024 || port > 49151)
        throw std::runtime_error("port out of range (1024-49151)");

    // faire des verrifications au niveau du mdp ?
    // -> pas dans le sujet mais si ca t'amuse
    return static_cast<uint16_t>(port);
}
