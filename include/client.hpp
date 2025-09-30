#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client
{
private:
    int fd_;
    std::string ip_;
    std::string nick_;
public:
    Client() {}

    int GetFD() const { return fd_; }
    void SetFD(int fd) { fd_ = fd; }
    void SetIpAddress(const std::string& ip) { ip_ = ip; }
};

#endif
