#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <iostream>
#include "server.hpp"
#include "colors.hpp"

void Server::Init() {
    HandleSignals();
    InitListeningSocket();
    InitEpollInstance();
    std::cout << "Server <" << socket_fd_ << "> " << GREEN << "Connected" << RESET << std::endl;
}

void Server::HandleSignals() {
    struct sigaction sa;
    memset(&sa, 0 , sizeof(sa));
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = Server::SignalHandler;
    sa.sa_flags = 0;

    if (sigaction(SIGTERM, &sa, NULL) == -1)
        throw std::runtime_error("failed to set sigaction for SIGTERM");
    if (sigaction(SIGQUIT, &sa, NULL) == -1)
        throw std::runtime_error("failed to set sigaction for SIGQUIT");
    if (sigaction(SIGINT, &sa, NULL) == -1)
        throw std::runtime_error("failed to set sigaction for SIGINT");
}

void Server::SignalHandler(int signum) {
    Server::running = false;
}

void Server::InitListeningSocket() {
    // === socket initialization ===
    // a socket is a form of IPC (inter process communication) just like
    // pipes(pipex), threads(philosopher) and signals(minitalk)

    // --- int socket(int domain, int type, int protocol); ---
    // System call used to create a socket. Returns an fd that refers to the new socket.
    // AF_INET      = IPv4
    // SOCK_STREAM  = TCP
    // O_NONBLOCK   = non-blocking
    // 0            = let the system choose the protocol
    socket_fd_ = socket(AF_INET, SOCK_STREAM | O_NONBLOCK, 0);
    if (socket_fd_ == -1) {
        throw std::runtime_error("failed to create server socket");
    }

    // --- int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen); ---
    // Set option on a socket.
    // SOL_SOCKET   = refers to socket-level option (other ex : IPPROTO_TCP IPPROTO_IP)
    // SO_REUSEADDR = allow immediate reuse of local addresses and ports.
    //                otherwise socket has to wait for TIME_WAIT delay
    // en           = a value that is used to know if option is active
    int en = 1;
    if (setsockopt(socket_fd_, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
        throw std::runtime_error("failed to set reuse address socket option");

    // --- int bind(int sockfd, const struct sockaddr addr, socklen_t addrlen); ---
    // Assigns an address and port to a socket to give him unique identity
    // from network's perspective. Without bind the socket can only receive
    // messages from processes on the same machine.
    // Note : integers are stored in a big endian way on the network they need
    // to be converted before being sent
    // htons = host byte order to network byte order short (uint16_t)
    // ntohs = network byte order to host byte order short (uint16_t)
    // AF_INET    = ipv4
    // INADDR_ANY = socket will bind to all available interfaces on the host machine
    struct sockaddr_in add;
    memset(&add, 0, sizeof(add));
    add.sin_family = AF_INET;
    add.sin_port = htons(port_);
    add.sin_addr.s_addr = INADDR_ANY;
    if (bind(socket_fd_, (struct sockaddr *)&add, sizeof(add)) == -1)
        throw std::runtime_error("failed to bind socket");

    // --- int listen(int sockfd, int backlog); ---
    // An active socket represents the client-side that sends messages.
    // A passive socket represents the server side that listens to messages.
    // The passive socket will create a new acite socket for every client that
    // connects so it can know who sent what.
    // To make the socket passive we use listen system call
    // SOMAXCONN = max number of pending connexion requests (system dependent)
    if (listen(socket_fd_, SOMAXCONN) == -1)
        throw std::runtime_error("failed to set socket on listening mode");
}

void Server::InitEpollInstance() {
    // Epoll is a way to tell linux kernel to survey a list of file descriptors.
    // We create an epoll instance and add our socket fd so everytime a client
    // tries to connect the epoll event loop will wake our server up
    epoll_fd_ = epoll_create1(0);
    if (epoll_fd_ == -1)
        throw std::runtime_error("failed to create epoll instance");

    // --- int epoll_ctl(int __epfd, int __op, int __fd, epoll_event *) ---
    // EPOLLIN       = detects incomming connexions (clients)
    // EPOLL_CTL_ADD = add the fd to the epoll instance
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN | EPOLLRDHUP;
    ev.data.fd = socket_fd_;
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, socket_fd_, &ev) == -1)
        throw std::runtime_error("failed to add socket fd to epoll");
}
