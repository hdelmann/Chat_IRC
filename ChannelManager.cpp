#include "ChannelManager.h"
#include "message_builder.h"
#include <iostream>
#include "defines.h"

ChannelManager::ChannelManager(void){
    
}
Channel  *ChannelManager::get_channel(std::string const &name){
    for (size_t i = 0; i < channels.size(); i++)
    {
        if(channels[i]->is_me(name))
        {
            return channels[i];
        }
    }
    return NULL;
}
int     ChannelManager::create(User &user, std::string const &chan_name){
    Channel *disney = new Channel(chan_name);
    this->channels.push_back(disney);
    disney->add_OP(user);
    disney->set_flags(MODE_t);
    disney->join(user);
    return 0;
}
void    ChannelManager::leave(User &user, std::string const message){
    
    for (std::vector<Channel*>::iterator it = this->channels.begin(); it < this->channels.end(); it++)
    {
        if ((*it)->is_user_present(user.get_nickname()) == 1)
        {
            (*it)->quit(user, message);
            if((*it)->members_count() == 0)
            {
                it = this->channels.erase(it);
                delete *it;
            }
        }
    }
    
}   
void    ChannelManager::leave(User &user, Channel &chan, std::string const message){
    chan.quit(user, message);
    for (std::vector<Channel*>::iterator it = this->channels.begin(); it < this->channels.end(); it++)
    {
        if (chan.members_count() == 0)
        {
            it = this->channels.erase(it);
            delete *it;
        }
    }
}
