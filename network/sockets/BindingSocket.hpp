#ifndef SIMPLESOCKET_HPP
#define SIMPLESOCKET_HPP

#include "SimpleSocket.hpp"

class SimpleSocket;

class BindingSocket : public SimpleSocket
{
	public :
		BindingSocket(int domain, int service, int protocol, int port, u_long interface);
		 virtual ~BindingSocket();
		int	connect_to_network(int sock, struct sockaddr_in adress);
};








#endif