#include "../../include/parsing.hpp"

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

// liste des ports interdits a 42 ???
static std::vector<int> portsAlreadyUsed = {
	// exemple :
	5432,
	8765
};

// voir les ports deja utilises :
// ss -tuln
// mettre un system qui empeche d'utiliser un port deja utiliser par 42 ?
int validateArgs(char const *argv[])
{
	 if (!isNumber( argv[1] ))
	{
		std::cout << "Error: invalid port (not a number)" << std::endl;
		return ( -1 );
	}

	int port = atoi( argv[1] );
	if (port < 1024 || port > 49151)
	{
		std::cout << "Error: port out of range (1024-49151)" << std::endl;
		return ( -1 );
	}

	// a voir ?
	if (isInTab( portsAlreadyUsed, port ))
	{
		std::cout << "Error: port already used" << std::endl;
		return ( -1 );
	}

	// faire des verrifications au niveau du mdp ?
	
	return ( port );
}