#include "Server.hpp"

void Server::CloseFds() {
    for (std::map<int, Client>::iterator it = unauthenticated_clients_.begin(); it != unauthenticated_clients_.end(); it++) {
        int fd = it->first;
        close(fd);
    }
    for (std::map<int, Client>::iterator it = connected_clients_.begin(); it != connected_clients_.end(); it++) {
        int fd = it->first;
        close(fd);
    }

    if (socket_fd_ != -1) {
        std::cout << "Server <" << socket_fd_ << "> " << RED << "Disconnected" << RESET << std::endl;
        close(socket_fd_);
        socket_fd_ = -1;
    }

    if (epoll_fd_ != -1) {
        close(epoll_fd_);
        epoll_fd_ = -1;
    }
}

void Server::Disconnect(int fd)
{
    bool show_message = connected_clients_.count(fd); //Only show message in server if User has been authenticated
    std::map<int, Client>& map = (show_message ? connected_clients_ : unauthenticated_clients_);

    if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, NULL) == -1) {
        std::cerr << "Error epoll_ctl: " << strerror(errno) << std::endl;
    }
    close(fd);
    map.erase(fd);
    if (show_message)
        std::cout << "Client <" << fd << "> " << RED << "Disconnected" << RESET << std::endl;
}