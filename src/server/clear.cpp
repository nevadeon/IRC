#include <unistd.h>
#include <vector>
#include "server.hpp"
#include "colors.hpp"

void Server::ClearClient(int fd) {
    //TODO clear fds from epoll
}

void Server::CloseFds() {
    for (std::vector<Client>::iterator it = clients_.begin(); it != clients_.end(); ++it) {
        int fd = it->GetFD();
        if (fd != -1) {
            std::cout << "Client <" << fd << "> " << RED << "Disconnected" << RESET << std::endl;
            close(fd);
        }
    }
    clients_.clear();
    if (socket_fd_ != -1) {
        std::cout << "Server <" << socket_fd_ << "> " << RED << "Disconnected" << RESET << std::endl;
        close(socket_fd_);
        socket_fd_ = -1;
    }
}
