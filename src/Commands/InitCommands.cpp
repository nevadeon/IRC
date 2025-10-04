#include "Server.hpp"

void Server::Commands::InitCommands()
{
    Server::Commands::commands["CAP"] = &Server::Commands::cap;
    Server::Commands::commands["PASS"] = &Server::Commands::pass;
    Server::Commands::commands["USER"] = &Server::Commands::user;
    Server::Commands::commands["PING"] = &Server::Commands::ping;
    Server::Commands::commands["PRIVMSG"] = &Server::Commands::privmsg;
    Server::Commands::commands["KICK"] = &Server::Commands::kick;
    Server::Commands::commands["INVITE"] = &Server::Commands::invite;
    Server::Commands::commands["JOIN"] = &Server::Commands::join;
    Server::Commands::commands["PART"] = &Server::Commands::part;
    Server::Commands::commands["QUIT"] = &Server::Commands::quit;
    Server::Commands::commands["TOPIC"] = &Server::Commands::topic;
    Server::Commands::commands["MODE"] = &Server::Commands::mode;
    Server::Commands::commands["NICK"] = &Server::Commands::nick;
}