#include "Commands.hpp"

/*
    CAP command can be ignored
*/
int Commands::cap(Server& server, int fd, std::vector<std::string>& args)
{
    (void)args;
    return 0;
}