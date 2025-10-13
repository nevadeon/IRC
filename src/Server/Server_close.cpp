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
    // std::map<Client*, operator_status> ch_clients_;
    // Client *client = &(clients_[fd]);
    for(std::map<std::string, Channel>::iterator it = channels_.begin(); it != channels_.end(); it++)
        std::cout << it->second.GetClients().erase(fd) << std::endl;
    // for(std::vector<Channel>::iterator it = channels_.begin(); it != channels_.end(); it++) {
        // std::map<Client *, operator_status> clients = it->GetClients();
        // for(std::map<Client *, operator_status>::iterator itcli = clients.begin(); itcli != clients.end(); itcli++) {
        //     std::cout << itcli->first->GetNick() << std::endl;
        // }
    // }
    clients_.erase(fd);
    std::cout << "Client <" << fd << "> " << RED << "Disconnected" << RESET << std::endl;
}