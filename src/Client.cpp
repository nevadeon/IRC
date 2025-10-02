#include "Client.hpp"

/*
    User can be authenticated once all conditions are filled
*/
Client::Client()
{
    valid_password_ = false;
    nick_.first = false;
    user_info_.first = false;
}