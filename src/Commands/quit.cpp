#include "Commands.hpp"

int Commands::quit(Server& server, int fd, std::vector<std::string>& args)
{
    (void)args; return 0;
}