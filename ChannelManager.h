#ifndef CHANNEL_MANAGER_H_
#define CHANNEL_MANAGER_H_

#include <string>
#include <vector>

#include "Channel.h"

class ChannelManager
{
    private:
        std::vector<Channel*>    channels;

    public:
        ChannelManager(void);

        Channel  *get_channel(std::string const &name);
        
        void    leave(User &user, std::string const message);
        void    leave(User &user, Channel &chan, std::string const message);

        int     create(User &user, std::string const &chan_name);
        
        ~ChannelManager(void){
            for (std::vector<Channel*>::iterator it = this->channels.begin(); it < this->channels.end(); it++)
                delete (*it);
        };
};

#endif
