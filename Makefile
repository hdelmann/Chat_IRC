NAME  = ircserv
CC    = c++
FLAGS = -Wextra -Wall -Werror -std=c++98 -g3 -O3 -pedantic
SRCS  = Channel.cpp \
		ChannelManager.cpp \
	    Message.cpp \
	    ServerCore.cpp\
		commands/commands.cpp \
	    User.cpp \
	    UserManager.cpp \
	    command_handler.cpp \
	    interface.cpp \
	    main.cpp
OBJS  = $(addprefix build/, $(SRCS:.cpp=.o))

all: ${NAME}

${NAME}: ${OBJS}
	${CC} ${FLAGS} ${OBJS} -o ${NAME}

./build/%.o: %.cpp
	mkdir -p $(shell dirname $@)
	${CC} ${FLAGS} -c $< -o $@

clean:
	rm -rf ./build

fclean: clean
	rm ircserv

re:		clean all

.PHONY:	all clean fclean re

