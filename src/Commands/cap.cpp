#include "Commands.hpp"

/*
    CAP command can be ignored
*/
int Commands::cap(Server& server, Client& sender, std::vector<std::string>& args)
{
    (void)args;
    return 0;
}