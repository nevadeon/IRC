#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h> //memset
#include <vector>
#include <iostream>
#include "server.hpp"
#include "client.hpp"
#include "colors.hpp"

static const int TIMEOUT = 1000;
static const int MAX_EVENTS = 64;
struct epoll_event epoll_events[MAX_EVENTS];

bool Server::running = true;
void Server::Run() {
    while (Server::running) {
        int n = epoll_wait(epoll_fd_, epoll_events, MAX_EVENTS, TIMEOUT);
        if (n == -1) {
            if (errno == EINTR) {
                std::cout << "Signal received start again" << std::endl;
                continue;
            }
            throw std::runtime_error("Epoll wait failed");
        }
        for (int i = 0; i < n; i++) {
            uint32_t event_flags = epoll_events[i].events;
            int fd = epoll_events[i].data.fd;

            if (event_flags & (EPOLLERR | EPOLLHUP)) {
                DisconnectClient(fd);
                continue;
            }
            if (fd == socket_fd_) {
                AcceptNewClients();
            } else {
                ReceiveNewData(fd);
            }
        }
    }
    CloseFds();
}

void Server::AcceptNewClients() {
    // we use a loop in case multiple clients try to connect at the same time
    while (true) {
        sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        int client_fd = accept(socket_fd_, (sockaddr *)&client_addr, &len);
        if (client_fd == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) { break; }
            if (errno == EINTR) { continue; }
            std::cerr << "Error accept: " << strerror(errno) << std::endl;
            break;
        }

        // we set client socket on non blocking
        int flags = fcntl(client_fd, F_GETFL, 0);
        if (flags == -1) {
            std::cerr << "Error fcntl get flags: " << strerror(errno) << std::endl;
            close(client_fd);
            continue;
        }
        if (fcntl(client_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
            std::cerr << "Error fcntl set flags: " << strerror(errno) << std::endl;
            close(client_fd);
            continue;
        }

        // we add client socket in the epoll instance
        struct epoll_event client_ev;
        memset(&client_ev, 0, sizeof(client_ev));
        client_ev.events = EPOLLIN | EPOLLRDHUP;
        client_ev.data.fd = client_fd;
        if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, client_fd, &client_ev) == -1) {
            std::cerr << "Error epoll: " << strerror(errno) << std::endl;
            close(client_fd);
            continue;
        }

        // add the new client to clients vector
        Client new_client;
        new_client.SetFD(client_fd);
        new_client.SetIpAddress(inet_ntoa(client_addr.sin_addr));
        clients_.push_back(new_client);

        std::cout << "Client <" << client_fd << "> " << GREEN << "Connected" << RESET << std::endl;
    }
}

void Server::ReceiveNewData(int fd) {
    char buffer[4096];

    // we use a loop in case data is bigger than buffer size
    while (true) {
        ssize_t nread = recv(fd, buffer, sizeof(buffer) - 1, 0);
        if (nread <= 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) { break; }
            if (errno == EINTR) { continue; }
            if (nread < 0) {
                std::cerr << "Error recv: " << strerror(errno) << std::endl;
            }
            DisconnectClient(fd);
            break;
        } else {
            buffer[nread] = '\0';
            std::cout << "Client <" << fd << "> sent: " << buffer;
            // TODO :
            // append data to per client buffer and extract complete lines (\n)
            // handle case when data is not complete (wait)
            // handle case when multiple commands are received in one read
        }
    }
}

void Server::DisconnectClient(int fd) {
    std::cout << "Client <" << fd << "> " << RED << "Disconnected" << RESET << std::endl;
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, NULL) == -1) {
        std::cerr << "Error epoll_ctl: " << strerror(errno) << std::endl;
    }
    for (std::vector<Client>::iterator it = clients_.begin(); it != clients_.end(); ++it) {
        if (it->GetFD() == fd) {
            clients_.erase(it);
            break;
        }
    }
    close(fd);
}
