#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include <unistd.h>
#include <poll.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

#include "User.h"
#include "UserManager.h"
#include "ServerCore.h"
#include "UserManager.h"

int	ft_str_is_numeric(char *str)

{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if ((str[i] >= '0') && (str[i] <= '9'))
		i++;
		else
			return (0);
	}
	return (1);
}

int main(int ac, char **av)
{
    if (ac == 3)
    {
        ServerCore sv(av[2]);
        if(ft_str_is_numeric(av[1]) == 1 && atoi(av[1]) >= 0 && atoi(av[1]) <= 65535)
            sv.loop(atoi(av[1]));
        else
        {
            std::cout << "Error: invalid port" << std::endl; 
            return 1;   
        }
    }
    else
        std::cout << "Usage : ./irc <port> <password>" << std::endl;
    return(1);
}
