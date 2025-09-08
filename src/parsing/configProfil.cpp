#include "parsing.hpp"
#include <iostream>

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
		if (!isAlNum(pseudo) && c != '_' && c != '-') {
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
			std::cout << RED << "Error : eof detected." << RESET << std::endl;
			throw std::runtime_error("");
		}

		if (dataValide( data ))
			return ( data );
		else
			std::cout << RED << "Error : Invalide Username. Try again." << RESET << std::endl;
	}
}
