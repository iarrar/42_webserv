# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bgonon <bgonon@student.42perpignan.fr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/13 23:38:10 by bgonon            #+#    #+#              #
#    Updated: 2024/03/25 09:46:55 by bgonon           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

RM = rm
RMFLAG = -f

SRCS = network/server/SimpleServer.cpp \
		network/server/Server.cpp \
		network/server/SetupServer.cpp \
		network/sockets/BindingSocket.cpp \
		network/sockets/ListeningSocket.cpp \
		network/sockets/SimpleSocket.cpp \
		network/utils/utils.cpp \
		network/utils/setup_conf.cpp \
		network/requests/BaseRequest.cpp \
		network/requests/Post.cpp \
		network/requests/Get.cpp \
		network/requests/Delete.cpp \
		network/requests/StatusCode.cpp \
		network/requests/MimeType.cpp \
		network/main.cpp \


OBJS := $(SRCS:.cpp=.o)

all: $(NAME)

.cpp.o: 
	$(CXX) $(CXXFLAGS) -c $< -o $(<:.cpp=.o)

$(NAME):  $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

leak : $(OBJS) 
	$(CXX) -g -fsanitize=address $(CXXFLAGS) -o $(NAME) $(OBJS)

gdb :
	$(CXX) -g $(SRCS) -o $(NAME)

clean:
	$(RM) $(RMFLAG) $(OBJS)

fclean: clean
	$(RM) $(RMFLAG) $(NAME) 

re: fclean all

.PHONY : all clean fclean re gdb

