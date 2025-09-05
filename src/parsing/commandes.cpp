#include "../../include/parsing.hpp"

// couleurs :
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_DEFAULT "\033[0m"

// KICK - Eject a client from the channel
// INVITE - Invite a client to a channel
// TOPIC - Change or view the channel topic
// MODE - Change the channel’s mode:
// · i: Set/remove Invite-only channel
// · t: Set/remove the restrictions of the TOPIC command to channel operators
// · k: Set/remove the channel key (password)
// · o: Give/take channel operator privilege


// fonctions prenant des parametres :
// ajouter une commande "help ?"
int kike(const char *argv);
int invite(const char *argv);
int topic(const char *argv);
int mode(const char *argv);
int join(const char *argv);
int msg(const char *argv);

static int (*commandes[])(const char *argv) = {
  &kike,
  &invite,
  &topic,
  &mode,
  &join, // join chanel
  &msg, // send message
};


int kike(const char *argv)
{
	(void)argv;
	return ( 0 );
}

int invite(const char *argv)
{
	(void)argv;
	return ( 0 );
}

int topic(const char *argv)
{
	(void)argv;
	return ( 0 );

}
int mode(const char *argv)
{
	(void)argv;
	return ( 0 );
}

int join(const char *argv)
{
	(void)argv;
	return ( 0 );
}

int msg(const char *argv)
{
	(void)argv;
	return ( 0 );
}

bool parseCommandes()
{
	std::string line;
	if (!std::getline( std::cin, line )) {
		std::cout << COLOR_RED << "Error : eof detected." << COLOR_DEFAULT << std::endl;
		return ( false );
	}
	

	// changer de solution pour split, strtok n'existe pas
	
	// const char *del = " "; // delemiteur entre chaque argument de la commande
	// std::vector<char *> argv; // les arguments de la commande
	// // Splitting the string based on the delimiter
	// char *arg = strtok(line.data(), del);
	// while (arg != nullptr) {
    //     std::cout << "\"" << arg << "\"" << " " << std::endl;

    //     // Get the next substring
    //     // argv.insert(arg);
	// 	arg = strtok(nullptr, del);
    // }
	// return ( true );


}
