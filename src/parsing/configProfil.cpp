#include "../../include/parsing.hpp"

// mettre des couleurs dans les messages d'erreurs pour + de lisibilite ?
// color
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_DEFAULT "\033[0m"

static bool isAlNum( const std::string& str )
{
	int i = 0;
	while (str[i])
	{
		if ((str[i] > '9' || str[i] < '0')
			&& (str[i] > 'z' || str[i] < 'a')
			&& (str[i] > 'Z' || str[i] < 'A'))
			return ( false );
		i++;
	}
	return ( true );
}

static bool dataValide( const std::string& pseudo )
{
	if (pseudo.empty())
		return ( false );
	for (char c : pseudo) {
		if (!isAlNum(pseudo)) {
			return ( false );
		}
	}
	return ( true );
}

//pseudo ou username
std::string getData( std::string type )
{
	std::string data;

	while (true) {
		std::cout << "Enter your " << type << " :" << std::endl;
		if (!std::getline( std::cin, data )) {
			// quand on ctrl+d
			std::cout << COLOR_RED << "Error : eof detected." << COLOR_DEFAULT << std::endl;
			return ( "" );
		}

		if (dataValide( data ))
			return ( data );
		else
			std::cout << COLOR_RED << "Error : Invalide Username. Try again." << COLOR_DEFAULT << std::endl;
	}
}