#include <cstdio>
#include <fcntl.h>
#include "SimpleSocket.hpp"
#include <iostream>


SimpleSocket::SimpleSocket(int domain, int service, int protocol, int port, u_long interface)
{
	//definition de la structure de l'adresse
	address.sin_family = domain;
	address.sin_port = htons(port); //htons converti le port en uint16_t ce qui converti les bytes dans l'ordre du network
	address.sin_addr.s_addr = htonl(interface); //htonl ici on va convertir notre adresse IP qui est en ordre pour notre systeme en ordre de byte pour le network 
 	//etablissement de la connexion
	sock = socket(domain, service, protocol);
	//bind (je crois que sur cette fonction on peut utiliser directement bind car connect est utilisee
	// pour le cote client et que le tuto essaye de faire une classe qui peut etre utilisee pour les deux)
	test_connection(sock);

	//connection = connect_to_network(sock, address);
}

void	SimpleSocket::test_connection(int item_to_test)
{
	if (item_to_test < 0)
	{
		perror("Failed to connect....");
		exit(EXIT_FAILURE);
	}


}

int	SimpleSocket::getSocket() const
{
	return (sock);
}

int SimpleSocket::getConnection() const
{
	return (connection);

}

struct sockaddr_in 	SimpleSocket::getAddressStruc() const 
{
	return (address);
}

SimpleSocket::~SimpleSocket() {}

// du coup, il va falloir extraire du fichier de confid domain, service, protocol, port et interface 




