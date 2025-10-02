#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

struct user_info
{
    std::string username;
    std::string hostname;
    std::string servername;
    std::string realname;
};

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
        bool valid_password_;
        std::pair<bool, struct user_info> user_info_;
        std::pair<bool, std::string> nick_;

    public:
        Client();

        int GetFD() const { return fd_; }
        void SetFD(int fd) { fd_ = fd; }
        void SetIpAddress(const std::string& ip) { ip_ = ip; }
        
        bool ValidNick() { return nick_.first; }
        std::string& GetNick() { return nick_.second; }
        void SetNick(std::string& nick) { nick_.first = true; nick_.second = nick; }
        bool GetPasswordState() { return valid_password_; }
        void ValidatePassword() { valid_password_ = true; }
};

#endif
