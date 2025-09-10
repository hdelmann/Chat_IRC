#ifndef DEFINES_H_
#define DEFINES_H_

typedef int                 socket_t;
typedef struct pollfd       pollfd_t;
typedef struct sockaddr_in  sockaddr_in_t;

#define BAD_PASS 0x1
#define PREFIX_SRV ":irc.42.fr"

#define MODE_i (1 << 0) 
#define MODE_t (1 << 1) 
#define MODE_k (1 << 2) 
#define MODE_o (1 << 3) 
#define MODE_l (1 << 4)
#define TIMEOUT 20

#endif
