#include "Client.hpp"

/*
    User can be authenticated once all conditions are filled
*/
Client::Client() : valid_password_(false), nick_(false, ""), is_authenticated_(false) { user_info_.first = false; }

void Client::ValidatePassword()
{
    this->valid_password_ = true;
    if (this->GetIfNicknameValidated() && this->GetUserInfoGiven())
        Authenticate();
}

void Client::SetUserInfo(struct user_info& user_info)
{
    user_info_.first = true;
    user_info_.second = user_info;
    if (GetIfNicknameValidated() && GetPasswordState())
        Authenticate();
}

void Client::SetNick(std::string& nick)
{
    nick_.first = true;
    nick_.second = nick;
    if (GetPasswordState() && GetUserInfoGiven())
        Authenticate();
}

void Client::Authenticate()
{
    this->is_authenticated_ = true;
}