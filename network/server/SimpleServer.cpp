#include "SimpleServer.hpp"


SimpleServer::SimpleServer(int domain, int service, int protocole, int port, u_long interface, int bklg)
{
	socket = new ListeningSocket(domain, service, protocole, port, interface, bklg);
}

ListeningSocket* SimpleServer::get_Socket()
{
	return (socket);
}