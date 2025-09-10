#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client
{
private:
    int _fd;
    std::string _ip;
public:
    Client() {}

    int GetFD() const { return _fd; }
    void SetFD(int fd) { _fd = fd; }
    void SetIpAddress(const std::string& ip) { _ip = ip; }
};

#endif
