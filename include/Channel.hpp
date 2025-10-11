#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>

const char symbols[] = {'i', 't', 'k', 'o', 'l'};

/*
    - Boolean values if operator(s) has/have decided to set the option
    - Limit set to -1 if unlimited amount of users can join, else limit is in place
*/
struct s_ch_info
{
    std::string ch_name;
    std::string ch_topic;
    std::string ch_key;
    int ch_user_limit;
};

/*
    .Channel name:
    - First character is # or &
    - No commas
    - No ASCII 7
    - No spaces
    - Length of up to 200 characters

    .Channel creation:
    - Doesnt exist: User JOINs channel, automatically becomes channel operator.
    - Exists: Depends on channel mode

    .Operators:
    - Have @ prefix 
*/
class Channel
{
    private:
        struct s_ch_info info_;
        std::map<char, bool> modes_;

    public:
        Channel(std::string& name);

        void SetMode(char c, bool mode);
        void SetTopic(std::string& str);
        void SetKey(std::string& str);
        void SetOperator(std::string& nickname);
        void SetLimit(int limit);
};

#endif