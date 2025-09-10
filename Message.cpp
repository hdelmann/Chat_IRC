#include <iostream>

#include "Message.h"

Message::Message(void){}

Message::Message(std::string const &raw_message){
    this->update(raw_message);
}

void Message::update(const std::string &raw_message){
    size_t i = 0;
    if (raw_message[i] == ':')
    {
        i = raw_message.find(' ', i);
    }
    this->command = raw_message.substr(i, raw_message.find(' ', i) -i);
    i = raw_message.find(' ', i);
    while (i < raw_message.size())
    {

        if(raw_message[i + 1] != ':')
        {
            i++;
            this->params.push_back(raw_message.substr(i, raw_message.find(' ', i) -i));
            i = raw_message.find(' ', i);
        }
        else
        {
            i++;
            this->params.push_back(raw_message.substr(i + 1, raw_message.size()));
            break;
        }
    }
}

std::string const &Message::get_command(void) const{
    return (this->command);
}

std::vector<std::string> const &Message::get_params(void)  const{
    return(this->params);
}

Message::~Message(void){}
