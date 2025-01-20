#include "ListeningSocket.hpp"
#include <iostream>

ListeningSocket::ListeningSocket(int domain, int service, int protocol, int port, u_long interface, int backlg) : BindingSocket(domain, service, protocol, port, interface)
{
    backlog = backlg;
    start_listening();
    test_connection(listening);
}

void    ListeningSocket::start_listening()
{
    listening = listen(getSocket(), backlog);
}

int     ListeningSocket::get_listening_socket()
{
    return(listening);
}

ListeningSocket::~ListeningSocket()
{

}
