#include "server.hpp"

void Server::CloseFds() {
    for (std::vector<Client>::iterator it = clients_.begin(); it != clients_.end(); ++it) {
        int fd = it->GetFD();
        if (fd != -1) {
            if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, NULL) == -1)
                std::cerr << "Error epoll_ctl: " << strerror(errno) << std::endl;
            close(fd);
            it->SetFD(-1);
            std::cout << "Client <" << fd << "> " << RED << "Disconnected" << RESET << std::endl;
        }
    }
    clients_.clear();

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
