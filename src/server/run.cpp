#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring> // memset
#include <vector>
#include "server.hpp"
#include "colors.hpp"

void Server::Run() {
    while (Server::running) {
        //TODO: add epoll io checking semantics
    }
    CloseFds();
}

void Server::ReceiveNewData(int fd) {
    char buffer[1024];

    memset(buffer, 0, sizeof(buffer));
    ssize_t nread = recv(fd, &buffer, sizeof(buffer) - 1, 0);

    if (nread <= 0) {
        std::cout << "Client <" << fd << "> " << RED << "Disconnected" << RESET << std::endl;
        ClearClient(fd);
        close(fd);
    } else {
        buffer[nread] = '\0';
        std::cout << "Client <" << fd << "> sent: " << buffer << std::endl;
        //TODO : add code that process data here
    }
}
