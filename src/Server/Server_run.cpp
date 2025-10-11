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
        clients_[client_fd] = new_client;

        // std::cout << "Client <" << client_fd << "> " << GREEN << "Connected" << RESET << std::endl;
    }
}

void Server::ReceiveNewData(int fd) {
    static char buffer[1024][4096];
    static int buffer_len[1024];
    static std::string str[1024];

    // test reply
    // std::vector<std::string> test;
    // test.push_back("gfsd");
    // test.push_back("gbvxcz");
    // this->Reply(fd, this->info_.name, std::string("987"), test);


    // we use a loop in case data is bigger than buffer size
    while (true) {
        ssize_t nread = recv(fd, buffer[fd], sizeof(buffer[fd]) - 1, 0);
        std::cout << buffer[fd] << std::endl;

        if (nread < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
                break;
            std::cerr << "Error recv: " << strerror(errno) << std::endl;
            Disconnect(fd);
            buffer[fd][0] = '\0';
            buffer_len[fd] = 0;
            break;
        } else if (nread == 0) {
            // normal deconnexion from client
            Disconnect(fd);
            buffer[fd][0] = '\0';
            buffer_len[fd] = 0;
            break;
        } else {
            buffer[fd][nread] = '\0';
            // TODO :
            // append data to per client buffer and extract complete lines (CRLF\r\n)
            // handle case when data is not complete (wait)
            // handle case when multiple commands are received in one read
            // parseInput(fd, buffer);
            str[fd] = str[fd].append(buffer[fd]);
            // std::cout << "Client<" << fd << ">: " << buffer[fd] << std::endl;
            // std::cout << "      buffer : <" << fd << ">: " << str[fd] << std::endl;
            buffer_len[fd] = strlen(buffer[fd]);
            std::cout << "test1\n";
            if (buffer_len[fd] > 1 && (buffer[fd][buffer_len[fd] - 1] == '\n')
                    && (buffer[fd][buffer_len[fd] - 2] == '\r')) {
                std::cout << "test2\n";
                ParseInput(fd, str[fd].data());
                buffer[fd][0] = '\0';
                buffer_len[fd] = 0;
                str[fd].clear();
            } else {
                str[fd].erase(str[fd].size() - 1);
            }
        }
    }
}
