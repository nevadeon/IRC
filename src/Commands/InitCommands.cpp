#include "Server.hpp"

void Server::Commands::InitCommands()
{
    Server::Commands::commands["CAP"] = &Server::Commands::CAP;
    Server::Commands::commands["PASS"] = &Server::Commands::PASS;
    Server::Commands::commands["USER"] = &Server::Commands::USER;
    Server::Commands::commands["PING"] = &Server::Commands::PING;
    Server::Commands::commands["PRIVMSG"] = &Server::Commands::PRIVMSG;
    Server::Commands::commands["KICK"] = &Server::Commands::KICK;
    Server::Commands::commands["INVITE"] = &Server::Commands::INVITE;
    Server::Commands::commands["JOIN"] = &Server::Commands::JOIN;
    Server::Commands::commands["PART"] = &Server::Commands::PART;
    Server::Commands::commands["QUIT"] = &Server::Commands::QUIT;
    Server::Commands::commands["TOPIC"] = &Server::Commands::TOPIC;
    Server::Commands::commands["MODE"] = &Server::Commands::MODE;
    Server::Commands::commands["NICK"] = &Server::Commands::NICK;
}