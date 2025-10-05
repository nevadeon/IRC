#include "Client.hpp"

/*
    User can be authenticated once all conditions are filled
*/
Client::Client() : valid_password_(false), nick_(false, ""), is_authenticated_(false) { user_info_.first = false; }