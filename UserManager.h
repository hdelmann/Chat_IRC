#ifndef USER_MANAGER_H_
#define USER_MANAGER_H_

#include <string>
#include <vector>

#include "User.h"

#include "defines.h"

class UserManager
{
    private:
        std::vector<User*>   users;
    
    public:
        UserManager(void);

        User    *get_user(std::string const &name);
        User    *get_user(int fd);

        std::vector<User*>::iterator begin(void);
        std::vector<User*>::iterator end(void);

        void    create_user(int fd);
		std::vector<User*>::iterator erase(std::vector<User*>::iterator it);

        void    check_pings(void);

        ~UserManager(void);
};

#endif
