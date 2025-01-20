#include "Server.hpp"
#include "../utils/utils.hpp"

#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <csignal>

bool Server::until = true;

Server::Server(int domain, int service, int protocole, std::vector<SetupServer *> SetupServers, int timeout)
{
	for (size_t i = 0; i < SetupServers.size(); ++i)
	{
		std::vector<int> ports = SetupServers[i]->getPort();
		for (size_t j = 0; j < ports.size(); ++j)
		{
			socket = new ListeningSocket(domain, service, protocole, ports[j], SetupServers[i]->getIp(), SetupServers[i]->getMaxConnections());
			if (socket->get_listening_socket() == -1)
			{
				std::cout << "Error getting listening socket" << std::endl;
				return ;
			}
			set_non_blocking(socket->getSocket());
			list_socket_server[socket] = SetupServers[i];
		}
	}
	if (timeout == 0)
		_timeout = -1;
	else
		_timeout = timeout;
	launch();
}
Server::Client::Client(std::map<ListeningSocket *, SetupServer *>::iterator server, int client) : server(server), fd_client(client){}

ListeningSocket* Server::get_Socket(std::map<ListeningSocket* , SetupServer*>::iterator it)
{
	return (it->first);
}


void Server::accept_connection(ListeningSocket *socket, std::map<ListeningSocket* , SetupServer*>::iterator it)
{
	(void)socket;
	struct sockaddr_in address = it->first->getAddressStruc();
	int addrlen = sizeof(address);
	new_socket = accept(it->first->getSocket(), (struct sockaddr *)&address, (socklen_t*)&addrlen);
	if (new_socket == -1)
	{
		perror("Error : Entering request failed");
	}
    else if (new_socket <= 2)
	{
        std::cout << "Warning: Invalid new socket descriptor" << new_socket << "\n" << std::endl;
        close(new_socket);
        return;
    }
	else
	{
		add_to_pollfds(pfds, &fd_count, &fd_size, new_socket, POLLIN);
		connections.push_back(Client(it, new_socket));
	}
}

void Server::handle_request(int sender_fd, std::map<ListeningSocket *, SetupServer *>::iterator listener, int i)
{
	request = new BaseRequest(sender_fd, listener->first->getSocket(), listener->second);
	if (request->getFindCloseConnection() != std::string::npos)
	{
		close(pfds[i].fd);
		remove_from_pollfds(pfds, &fd_count, sender_fd);
		delete request;
		return ;
	}
	else if (request->getNbytes() <= 0)
	{
		if (request->getNbytes() == 0)
		{
			close(pfds[i].fd);
			remove_from_pollfds(pfds, &fd_count, sender_fd);
			delete request;
			return ;
		}
		else
		{
			close(pfds[i].fd);
			remove_from_pollfds(pfds, &fd_count, sender_fd);
			delete request;
			return ;
		}
	}
	delete request;
}

void Server::handle_signal(int signum)
{
	(void)signum;
	std::cout << "Closing server please wait..." << std::endl;
	until = false;
}

void Server::cleanup()
{
	int i = 0;
	std::vector<ListeningSocket *> ls;
	std::vector<SetupServer *> ss;
	
     for (std::map<ListeningSocket *, SetupServer *>::iterator it = list_socket_server.begin(); it != list_socket_server.end(); ++it)
	 {
		ls.push_back(it->first);
		ss.push_back(it->second);
		i++;

	 }
	size_t j = 0;
	while (j != ls.size())
	{
		delete ls[j];
		j++;
		
	}
	return ;
}

void	Server::launch()
{
	init_pollfds(pfds, &fd_count, &fd_size);
	
	fd_count = 0;
	signal(SIGINT, handle_signal);
	signal(SIGTERM, handle_signal);
	std::map<ListeningSocket* , SetupServer*>::iterator it;
	size_t i = 0;
	for(it = list_socket_server.begin(); it != list_socket_server.end(); ++it)
	{
		pfds[i].fd = it->first->getSocket();
		pfds[i].events = POLLIN;
		i++;
	}
	i = 0;
	fd_count = list_socket_server.size();
	while (until)
	{
		int poll_count = poll(pfds.data(), fd_count, _timeout);
		if (poll_count == 0)
			continue;
		if (poll_count == -1)
		{
			break ;
		}
		for(int i = 0; i < fd_count; i++)
		{
			if (pfds[i].revents & POLLIN)
			{
                bool is_server_socket = false;
				for (it = list_socket_server.begin(); it != list_socket_server.end(); ++it)
				{
					if(pfds[i].fd == it->first->getSocket())
					{
						accept_connection(it->first, it);
						is_server_socket = false;
					}
				}
				if(!is_server_socket)
				{
					for(size_t j = 0; j < connections.size(); ++j)
					{
						if(connections[j].fd_client == pfds[i].fd)
						{
							handle_request(pfds[i].fd, connections[j].server, i);
						}
					}
				}
			}
		}
	}
	cleanup();
}