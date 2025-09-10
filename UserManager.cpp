#include <iostream>

#include "UserManager.h"

UserManager::UserManager(void)
{
}

User *UserManager::get_user(std::string const &name)
{
    for (unsigned long i = 0; i < this->users.size(); i++)
    {
        if (users[i]->is_me(name))
            return (this->users[i]);
    }
    return (NULL);
}

User *UserManager::get_user(int fd)
{
    for (unsigned long i = 0; i < this->users.size(); i++)
    {
        if (this->users[i]->is_me(fd))
            return (this->users[i]);
    }
    return (NULL);
}

void UserManager::create_user(int fd)
{
    User *user = new User (fd);
    user->set_state(User::WAITING_FOR_PASS);
    this->users.push_back(user);
}

std::vector<User*>::iterator UserManager::erase(std::vector<User*>::iterator it){
    delete *it;
    return this->users.erase(it);
}

std::vector<User*>::iterator UserManager::begin(void)
{
    return this->users.begin();
}

std::vector<User*>::iterator UserManager::end(void)
{
    return this->users.end();
}

void UserManager::check_pings(void)
{
    for (unsigned long i = 0; i < this->users.size(); i++)
    {        
        if (this->users[i]->get_last_ping() <= time(NULL)-30)
        {
            this->users[i]->send_ping();
        }
        else if(this->users[i]->get_delta() >= 60)
        {
            this->users[i]->send_message("ERROR :Timeout", false);
            this->users[i]->set_state(User::WAITING_FOR_QUIT);
        }
    }
}

UserManager::~UserManager(void) {
    for (std::vector<User*>::iterator it = this->users.begin(); it < this->users.end(); it++)
        delete *it;
}
