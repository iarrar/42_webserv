#include <cstdio>
#include <netinet/in.h>
#include <sys/socket.h> //librairie pour socket
#include <cstdlib>

class SimpleSocket
{
	public :
		SimpleSocket(int domain, int service, int protocol, int port, u_long interface);
		//domain = type d'adresse ip utilisee IPv4 ou IPv6 ?
		//service = TCP ou UDP ?
		//protocol = 
		//port = port en int ex 8080
		virtual int connect_to_network(int sock, struct sockaddr_in address) = 0;
		void test_connection(int );
		int		getSocket() const;
		int		getConnection() const;
		struct sockaddr_in getAddressStruc() const;
		virtual ~SimpleSocket();
		
	protected :
		struct sockaddr_in	address; //cf info.cpp
		int connection; 
		int sock; //fd du socket

};

