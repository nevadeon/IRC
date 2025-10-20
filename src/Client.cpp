#include "Client.hpp"
#include <iostream>

/*
    User can be authenticated once all conditions are filled
*/
Client::Client() : valid_password_(false), nick_(false, "*"), is_authenticated_(false) { user_info_.first = false; }

//returns 1 if server needs to welcome Client
int Client::ValidatePassword()
{
    valid_password_ = true;
    if (!is_authenticated_ && GetIfNicknameValidated() && GetUserInfoGiven())
    {
        Authenticate();
        return (1);
    }
    return (0);
}

int Client::SetUserInfo(struct user_info& user_info)
{
    user_info_.first = true;
    user_info_.second = user_info;
    if (!is_authenticated_ && GetIfNicknameValidated() && GetPasswordState())
    {
        Authenticate();
        return (1);
    }
    return (0);
}

int Client::SetNick(std::string& nick)
{
    nick_.first = true;
    nick_.second = nick;
    if (!is_authenticated_ && GetPasswordState() && GetUserInfoGiven())
    {
        Authenticate();
        return (1);
    }
    return (0);
}

void Client::Authenticate()
{
    is_authenticated_ = true;
}