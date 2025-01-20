#ifndef SIMPLESERVER_HPP
#define SIMPLESERVER_HPP

#include "../sockets/ListeningSocket.hpp"
#include <map>
#include <vector>

class SimpleServer
{
	public :
		SimpleServer(int domain, int service, int protocol, int port, u_long interface, int bklg);
		virtual void launch() = 0;
		ListeningSocket*	get_Socket();
	private :
		ListeningSocket	*socket;
		virtual void accepter() = 0;
		virtual void handler() = 0;
		virtual void responder() = 0;


};




#endif