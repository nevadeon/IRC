#include "Server.hpp"

static const int TIMEOUT = 1000;
static const int MAX_EVENTS = 64;
struct epoll_event epoll_events[MAX_EVENTS];

volatile std::sig_atomic_t Server::running = true;
void Server::Run() {
    while (Server::running) {
        int n = epoll_wait(epoll_fd_, epoll_events, MAX_EVENTS, TIMEOUT);
        if (n == -1) {
            if (errno == EINTR) { continue; }
            throw std::runtime_error("Epoll wait failed");
        }
        for (int i = 0; i < n; i++) {
            uint32_t event_flags = epoll_events[i].events;
            int fd = epoll_events[i].data.fd;

            if (event_flags & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
                Disconnect(fd);
                continue;
            }
            if (fd == socket_fd_) {
                AcceptNewConnections();
            } else {
                ReceiveNewData(fd);
            }
        }
    }
    CloseFds();
}

void Server::AcceptNewConnections() {
    // we use a loop in case multiple clients try to connect at the same time
    while (true) {
        sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        int client_fd = accept(socket_fd_, (sockaddr *)&client_addr, &len);
        if (client_fd == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) { break; }
            std::cerr << "Error accept: " << strerror(errno) << std::endl;
            break;
        }

        // set client socket on non blocking
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

        // add client socket in the epoll instance
        struct epoll_event client_ev;
        memset(&client_ev, 0, sizeof(client_ev));
        client_ev.events = EPOLLIN | EPOLLRDHUP;
        client_ev.data.fd = client_fd;
        if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, client_fd, &client_ev) == -1) {
            std::cerr << "Error epoll: " << strerror(errno) << std::endl;
            close(client_fd);
            continue;
        }

        Client new_client;
        new_client.SetFD(client_fd);
        new_client.SetIpAddress(inet_ntoa(client_addr.sin_addr));
        unauthenticated_clients[client_fd] = new_client;

        // std::cout << "Client <" << client_fd << "> " << GREEN << "Connected" << RESET << std::endl;
    }
}

void Server::ReceiveNewData(int fd) {
    char buffer[4096];

    // we use a loop in case data is bigger than buffer size
    while (true) {
        ssize_t nread = recv(fd, buffer, sizeof(buffer) - 1, 0);
        if (nread < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) { break; }
            std::cerr << "Error recv: " << strerror(errno) << std::endl;
            Disconnect(fd);
            break;
        } else if (nread == 0) {
            // normal deconnexion from client
            Disconnect(fd);
            break;
        } else {
            buffer[nread] = '\0';
            if (buffer[nread - 1] == '\n')
                buffer[nread - 1] = '\0';
            // TODO :
            // append data to per client buffer and extract complete lines (CRLF\r\n)
            // handle case when data is not complete (wait)
            // handle case when multiple commands are received in one read
            // parseInput(fd, buffer);
            std::cout << "Client<" << fd << ">: " << buffer << std::endl;
            ParseInput(fd, buffer);
        }
    }
}


/*
    This function is needed because
    clients are uniquely identified by nickname
    but we receive events through their socket fd.
*/
Client* Server::FindClientByFD(int fd)
{
    for (std::map<std::string, Client>::iterator it = connected_clients_.begin(); it != connected_clients_.end(); it++)
    {
        if (it->second.GetFD() == fd)
            return &it->second;
    }
    return NULL;
}