#include <iostream>
#include <stdio.h>
#include <cstdlib>

#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>


#include "ServerCore.h"
#include "command_handler.h"
#include "commands/commands.h"

ServerCore::ServerCore(std::string const &password) : _password(password)
{
}

const std::string &ServerCore::get_password(void) const
{
    return this->_password;
}

ChannelManager    &ServerCore::get_channelManager(void){
    return (this->_channel_manager);
}

UserManager &ServerCore::get_userManager(void){
    return (this->_user_manager);
}

int quit = 0;
void  bshandler(int bs){
    (void)bs;
    quit = 1;
    
    return;
}

void ServerCore::loop(int port)
{
    std::vector<pollfd_t> pfds;
    sockaddr_in ipv4;
    memset(&ipv4, 0, sizeof(ipv4));
    int fd = 0;
    int ret = 0;
    int setsock = 1;
    signal(SIGINT, bshandler);
    ipv4.sin_family = AF_INET;
    ipv4.sin_port = htons(port);
    ipv4.sin_addr.s_addr = INADDR_ANY;
    ret = fd = socket(AF_INET, SOCK_STREAM, 0);
    ret |= setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &setsock, sizeof(setsock));
    ret |= bind(fd, (sockaddr *)&ipv4, sizeof(ipv4));
    ret |= listen(fd, SOMAXCONN);
    if (ret < 0)
    {
        perror("Error");
        exit(1);
    }
    {
        pollfd tmpfd;
        memset(&tmpfd, 0, sizeof(tmpfd));
        tmpfd.fd = fd;
        tmpfd.events = POLLIN;
        pfds.push_back(tmpfd);
    }
    while (quit == 0)
    {
        poll(pfds.data(), pfds.size(), 10000);

        for (size_t i = 0; i < pfds.size(); i++)
        {
            if ((pfds[i].revents & POLLIN) == 0)
            {
                continue;
            }
            if (pfds[i].fd == fd)
            {
                pollfd tmpfd;
                sockaddr addr;
                memset(&tmpfd, 0, sizeof(tmpfd));
                socklen_t a;
                memset(&a, 0, sizeof(a));
                tmpfd.fd = accept(pfds[i].fd, (sockaddr *)&addr, &a);
                tmpfd.events = POLLIN;
                pfds.push_back(tmpfd);
                this->_user_manager.create_user(tmpfd.fd);
            }
            else
            {
                User *client = this->_user_manager.get_user(pfds[i].fd);
                try
                {
                    client->recvu();
                    while (client->is_message_buffered())
                    {
                        Message msg;
                        client->get_message(msg);
                        handle_command(*client, msg, *this);
                    }
                }
                catch (const std::exception &e)
                {
                    perror("error");
                    client->set_state(User::WAITING_FOR_QUIT);    
                    std::cerr << e.what() << std::endl;
                }
            }
        }
        _user_manager.check_pings();
        this->disconnect_clients(pfds);
    }
}

void	ServerCore::disconnect_clients(std::vector<pollfd_t> &pfd)
{
    std::vector<User*>::iterator it = this->_user_manager.begin();
    while (1)
    {
        if(it == this->_user_manager.end())
        {
            break;
        }
        if ((*it)->is_state(User::WAITING_FOR_QUIT))
        {
            this->_channel_manager.leave(**it,  "YOU GOT DISCONECTED");
            pfd.erase(pfd.begin() + std::distance(this->_user_manager.begin(), it) + 1);
            (*it)->close_connection();
            it = this->_user_manager.erase(it);
            continue;
        }
        it++;
    }
}
