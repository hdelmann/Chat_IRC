#ifndef SERVERCORE_H_
#define SERVERCORE_H_

#include <string>

#include "ChannelManager.h"
#include "UserManager.h"

class ServerCore
{
    private:
        std::string const   _password;
        ChannelManager      _channel_manager;
        UserManager         _user_manager;
        
		void				disconnect_clients(std::vector<pollfd_t> &poll_fds);

    public:
        ServerCore(std::string const &password);

        const std::string &get_password(void) const;
        ChannelManager    &get_channelManager(void);
        UserManager       &get_userManager(void);

        void    loop(int listen_port);

        ~ServerCore(void){
            
        };
};

#endif
