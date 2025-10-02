#include <iostream>
#include "ParseUtil.hpp"
#include "Server.hpp"

// 0 pour admin et 4 pour user
int typeUser = 5;

int main(int argc, char const *argv[])
{
    if (argc != 3) {
        std::cerr << "Expected format : ./ircserv <port> <password>" << std::endl;
        return 0;
    }

    uint16_t port;
    try {
        port = Util::parsePort(argv);
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl; return 0;
    }

    Server server(port, argv[2]);
    try {
        server.Init();
        server.Run();
    } catch (const std::exception& e) {
        server.CloseFds();
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
