#include "BindingSocket.hpp"
#include <iostream>
#include <cerrno>

BindingSocket::BindingSocket(int domain, int service, int protocol, int port, u_long interface) : SimpleSocket(domain, service, protocol, port, interface)
{
	int connection = connect_to_network(getSocket(), getAddressStruc());
	(void)connection;
}

int 	BindingSocket::connect_to_network(int sock, struct sockaddr_in address)
{
	int result = bind(sock, (struct sockaddr *)&address, sizeof(address));
    if (result == -1)
    {
        return result;
        // std::cout << "Port already in use, new connectiom added " << getSocket() << std::endl;
    }
    return result;
	//return (bind(sock, (struct sockaddr *)& address, sizeof(adress)));
}

BindingSocket::~BindingSocket() {}