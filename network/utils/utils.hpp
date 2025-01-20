#ifndef UTILS_HPP
# define UTILS_HPP

# include <fcntl.h>
# include <iostream>
# include <stdio.h>
# include <stdlib.h>
# include <poll.h>
# include <unistd.h>
# include <vector>
# include <poll.h>

std::string getRideOfPlus(std::string pageContent);
void    init_pollfds(std::vector<struct pollfd>& pfds, int *fd_count, int *fd_size);
void    add_to_pollfds(std::vector<struct pollfd>& pfds, int *fd_count, int *fd_size, int nfd, short events);
void    remove_from_pollfds(std::vector<struct pollfd>& pfds, int *fd_count, int fd);
int     is_fd_ready(std::vector<struct pollfd>& pfds, int fd_count, int fd, short event);
void    set_non_blocking(int fd);
void    handle_signal(int signal);

#endif