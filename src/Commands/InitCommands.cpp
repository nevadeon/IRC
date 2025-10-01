#include "Commands.hpp"

void Commands::InitCommands()
{
    Commands::commands["CAP"] = &Commands::cap;
    Commands::commands["PASS"] = &Commands::pass;
    Commands::commands["USER"] = &Commands::user;
    Commands::commands["PING"] = &Commands::ping;
    Commands::commands["PRIVMSG"] = &Commands::privmsg;
    Commands::commands["KICK"] = &Commands::kick;
    Commands::commands["INVITE"] = &Commands::invite;
    Commands::commands["JOIN"] = &Commands::join;
    Commands::commands["PART"] = &Commands::part;
    Commands::commands["QUIT"] = &Commands::quit;
    Commands::commands["TOPIC"] = &Commands::topic;
    Commands::commands["MODE"] = &Commands::mode;
}