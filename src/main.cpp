#include <iostream>
#include "parsing.hpp"

// essayer avec des try catch plutot ?
// -> yes les trycatch c'est idiomatic au cpp
// la plupart des lib standard utilisent trycatch donc c'est plus facile a integrer

// 0 pour admin et 4 pour user
int typeUser = 5;

int main(int argc, char const *argv[])
{
	if (argc != 3)
	{
		std::cerr << "Expected format : ./ircserv <port> <password>" << std::endl;
		return 0;
	}
	try { validateArgs(argv); } catch (std::exception& e) {
		std::cerr << "Error :" << e.what() << std::endl; return 0;
	}

	// TODO serveur :
	// gethostname pour recup le nom de l'host
	// init socket en non blockant
	// link socket a l'ip du serveur avec la fonction bind
	// fcntl en non blockant
	// setup struct socketaddr_in avec le port
	// ecoute le socket avec listen
	// accepter des clients avec une loop infinie
	// recup le fd des clients avec epool


	// connexion au serveur

	// Je crois qu'on ne dois pas faire cette partie comme ca
	// dans le sujet ils disent :
	// 	Afin de vous assurer que votre serveur traite tout ce que vous lui envoyez, voici un
	// test basique qui peut être fait avec nc :
	// \$> nc 127.0.0.1 6667
	// com^Dman^Dd
	// \$>
	// Utilisez ctrl+D pour envoyer la commande en plusieurs parties : ’com’, puis ’man’,
	// puis ’d\n’.
	// ce que je comprends : un nouvel utilisateur se connecte via une commande qui
	// est parse d'un seul coup
	// ce n'est pas le serveur qui prompt l'utilisateur pour se connecter
	// le client se connecte au socket
	// le client envoie un paquet d'authentification et le serveur repond OK ou ERROR

	// explication
	// std::cout << "Use /help for more informations" << std::endl;
	// std::cout << "Use /join <id> for join a chanel, /msg <msg> for send message to the chanel or /msg <username> <msg> for send message to username" << std::endl;

	try { parseCommandes(); } catch (std::exception& e) {
		std::cerr << RED << "Error :" << e.what() << RESET << std::endl; return 0;
	}

	return 0;
}
