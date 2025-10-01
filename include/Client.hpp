#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

/*
    Information regarding the client
    specified by the associated file descriptor.

*/
class Client
{
    private:
        int fd_;
        std::string ip_;
        std::string nick_;
        bool is_authenticated_;
    public:
        Client() : is_authenticated_(false) {}

        int GetFD() const { return fd_; }
        void SetFD(int fd) { fd_ = fd; }
        void SetAuthenticated() { is_authenticated_ = true; }
        void SetIpAddress(const std::string& ip) { ip_ = ip; }

        bool operator==(Client& c2) {return (this->nick_ == c2.nick_);}
};

#endif
