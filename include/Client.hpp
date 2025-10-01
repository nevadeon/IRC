#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

/*
    Information regarding the client
    specified by the associated file descriptor.
    Each client is uniquely identify by its

*/
class Client
{
    private:
        int fd_;
        std::string ip_;
        std::string nick_;
    public:
        Client() {}

        std::string& GetNick() { return nick_; }
        void SetNick(std::string& nick) { nick_ = nick; }
        int GetFD() const { return fd_; }
        void SetFD(int fd) { fd_ = fd; }
        void SetIpAddress(const std::string& ip) { ip_ = ip; }

        bool operator==(Client& c2) {return (this->nick_ == c2.nick_);}
};

#endif
