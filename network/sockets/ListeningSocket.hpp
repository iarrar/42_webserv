#ifndef LISTENINGSOCKET_HPP
#define LISTENINGSOCKET_HPP

#include "BindingSocket.hpp"


class ListeningSocket : public BindingSocket
{
    public :
        ListeningSocket(int domain, int service, int protocol, int port, u_long interface, int backlg);
        virtual ~ListeningSocket();
        void    start_listening();
        int     get_listening_socket();
    private :
        int backlog;
        int listening;

};


#endif