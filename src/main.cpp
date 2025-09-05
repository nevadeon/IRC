#include <iostream>
#include "../include/parsing.hpp"


// essayer avec des try catch plutot ?

// 0 pour admin et 4 pour user
int typeUser = 5;

int main(int argc, char const *argv[])
{
	if (argc != 3)
	{
		std::cout << "Expected format : ./ircserv <port> <password>" << std::endl;
		return ( 0 );
	}
	if(validateArgs(argv) == -1)
		return ( 0 );

	// connexion au serveur

	// mettre ca dans la partie communuication avec le serveur :
	std::string username;
	std::string pseudo;
	//config profil :
	// username
	if ((username = getData("username")) == "")
		return ( 0 );
	// pseudo
	if ((pseudo = getData("pseudo")) == "")
		return ( 0 );

	// explication
	// std::cout << "Use /help for more informations" << std::endl;
	// std::cout << "Use /join <id> for join a chanel, /msg <msg> for send message to the chanel or /msg <username> <msg> for send message to username" << std::endl;
	
	if (!parseCommandes())
		return ( 0 );


	

	return 0;
}
