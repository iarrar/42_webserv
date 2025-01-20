#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <unistd.h>
#include <vector>
#include "utils.hpp"

std::string getRideOfPlus(std::string pageContent)
{
    std::string finalPageContent = pageContent;
    for(size_t i = 0; i < pageContent.size(); i++)
    {
        if(pageContent[i] == '+')
            finalPageContent[i] = ' ';
    }
    return(finalPageContent);
}

void init_pollfds(std::vector<struct pollfd>& pfds, int *fd_count, int *fd_size)
{
	*fd_count = 0;
	*fd_size = 100;
	pfds.resize(*fd_size);
}

void add_to_pollfds(std::vector<struct pollfd>& pfds, int *fd_count, int *fd_size, int nfd, short events)
{
	if (*fd_count == *fd_size)
	{
		*fd_size += 10;
		std::cout << "Too much connections, added 10 slots for new fd, new size: " << *fd_size << std::endl;
		pfds.resize(*fd_size);
	}
	(pfds)[*fd_count].fd = nfd;
	(pfds)[*fd_count].events = events;
	(pfds)[*fd_count].revents = 0;
	// std::cout << "New connection, fd is " << (pfds)[*fd_count].fd << " and event of the connection is " << (pfds)[*fd_count].events << std::endl;
	(*fd_count)++;
}

void remove_from_pollfds(std::vector<struct pollfd>& pfds, int *fd_count, int fd)
{
	for (int i = 0; i < *fd_count; i++)
	{
		if (pfds[i].fd == fd)
		{
			pfds[i] = pfds[(*fd_count) - 1];
			pfds.pop_back();
			(*fd_count)--;
			break ;
		}
	}
}

int is_fd_ready(std::vector<struct pollfd>& pfds, int fd_count, int fd, short event) {
	for (int i = 0; i < fd_count; i++)
	{
		if (pfds[i].fd == fd && (pfds[i].revents & event))
			return 1;
	}
	return 0;
}

void set_non_blocking(int socket)
{
	int flags = fcntl(socket, F_GETFL, 0);
	fcntl(socket, F_SETFL, flags | O_NONBLOCK);
}

void handle_signal(int signal)
{
	(void)signal;
	exit(1);
}