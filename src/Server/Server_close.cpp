#include "Server.hpp"

void Server::CloseFds() {
    for (std::map<int, Client>::iterator it = unauthenticated_clients.begin(); it != unauthenticated_clients.end(); ++it) {
        int fd = it->first;
        Disconnect(fd);
    }

    if (socket_fd_ != -1) {
        std::cout << "Server <" << socket_fd_ << "> " << RED << "Disconnected" << RESET << std::endl;
        if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, socket_fd_, NULL) == -1)
            std::cerr << "Error epoll_ctl: " << strerror(errno) << std::endl;
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
    if (unauthenticated_clients.count(fd))
        DisconnectUnauthenticated(fd);
    else
        DisconnectAuthenticated(*FindClientByFD(fd));
}

void Server::DisconnectUnauthenticated(int fd)
{
    unauthenticated_clients.erase(fd);
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, NULL) == -1) {
        std::cerr << "Error epoll_ctl: " << strerror(errno) << std::endl;
    }
    close(fd);
    std::cout << "Client <" << fd << "> " << RED << "Disconnected" << RESET << std::endl;
}

void Server::DisconnectAuthenticated(Client& c)
{
    std::string c_nick = c.GetNick();
    int c_fd = c.GetFD();
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, c_fd, NULL) == -1) {
        std::cerr << "Error epoll_ctl: " << strerror(errno) << std::endl;
    }
    if (connected_clients_.count(c_nick))
        connected_clients_.erase(c_nick);
    close(c_fd);
    std::cout << "Client <" << c_nick << "> " << RED << "Disconnected" << RESET << std::endl;
}
