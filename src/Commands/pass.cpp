#include "Commands.hpp"

int Commands::pass(Server& server, int fd, std::vector<std::string>& args)
{
    std::string password = args[1];
    std::string serv_password = server.GetPassword();

    switch (password == serv_password)
    {
        case 1:
            if (server.GetAuthenticatedClients().count(fd))
            {
                // 434 ERR_ALREADYREGISTERED
            }
            server.GetUnauthenticatedClients()[fd].ValidatePassword();

        case 0:
            // 464 ERR_PASSWDMISMATCH
            server.Disconnect(fd);
            break;
    }
}