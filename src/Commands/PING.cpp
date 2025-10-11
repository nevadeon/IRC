#include "Server.hpp"

int Server::Commands::PING(Server& server, int fd, std::vector<std::string>& args)
{
    std::vector<std::string> params;
    std::string returnValue = "PONG";
    if (args.size() > 1)
        params.push_back(args[1]);
    server.Reply(fd, server.info_.servername, returnValue, params);
    return 0;
}