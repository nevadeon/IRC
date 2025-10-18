#include "Server.hpp"

void Server::CloseFds() {
    for (std::map<int, Client>::iterator it = clients_.begin(); it != clients_.end(); it++) {
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
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, NULL) == -1) {
        std::cerr << "Error epoll_ctl: " << strerror(errno) << std::endl;
    }
    if (close(fd) < 0)
        std::cerr << "Error close: " << strerror(errno) << std::endl;
    for(std::map<std::string, Channel>::iterator it = channels_.begin(); it != channels_.end();) {
        std::map<int, operator_status> &clients = it->second.GetClients();
        clients.erase(fd);
        if (clients.empty()) {
            std::map<std::string, Channel>::iterator toErase = it++;
            channels_.erase(toErase);
        } else {
            ++it;
        }
    }
    clients_.erase(fd);
    std::cout << "Client <" << fd << "> " << RED << "Disconnected" << RESET << std::endl;
}