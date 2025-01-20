#include <netinet/in.h>

struct sockaddr_in
{
	short int 				sin_family; //famille d'adresse (AF_INET)
	unsigned short int		sib_port;	// Port (en format reseau)
	struct 					in_addr sin_addr;	//Adresse IPv4
	unsigned char			sin_zero[8];		//Remplissage pour correspondre a la taille de sockaddr
};


struct in_addr
{
	uint32_t s_addr; //Adresse IPv$ en format reseau
};

struct sockaddr_in6 {
    u_int16_t sin6_family;      // Famille d’adresses (AF_INET6)
    u_int16_t sin6_port;        // Port (en format réseau)
    u_int32_t sin6_flowinfo;    // Informations de flux IPv6
    struct in6_addr sin6_addr;  // Adresse IPv6
    u_int32_t sin6_scope_id;    // Identifiant de portée (interface)
};

//listening

//int listen(int socket, int backlog); //backlog = nombre de client que le serveur eut servir

/***/
//poll()
// struct pollfd {
//     int fd;         // the socket descriptor
//     short events;   // bitmap of events we're interested in
//     short revents;  // when poll() returns, bitmap of events that occurred
// };
// For events and revents :
// POLLIN : There is data to read.
// POLLRDNORM : Normal data can be read.
// POLLPRI : There is urgent data to read.
// POLLOUT : Writing is now possible.
// POLLWRNORM : Normal data can be written.
// POLLERR : An error has occurred.
// POLLHUP : The file descriptor has been hung up.
// POLLNVAL : The file descriptor is invalid.
/***/