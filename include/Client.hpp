#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client
{
private:
    int fd_;
    std::string ip_;
    std::string nick_;
    bool is_authenticated;
public:
    Client() : is_authenticated(false) {}

    int GetFD() const { return fd_; }
    void SetFD(int fd) { fd_ = fd; }
    void SetAuthenticated() { is_authenticated = true; }
    void SetIpAddress(const std::string& ip) { ip_ = ip; }
};

#endif
