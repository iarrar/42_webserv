#ifndef SERVER_HPP
#define SERVER_HPP

#include "SetupServer.hpp"
#include "../sockets/ListeningSocket.hpp"
#include "../requests/BaseRequest.hpp"
#include <string>
#include <vector>
#include <poll.h>

class Server
{
private:
    void accept_connection(ListeningSocket *socket, std::map<ListeningSocket* , SetupServer*>::iterator it);
    void handle_request(int sender_fd, std::map<ListeningSocket *, SetupServer *>::iterator listener, int i);
    void cleanup();

    static bool until;
    int new_socket;
    int _timeout;
    BaseRequest *request;
    ListeningSocket *socket;
    std::map<ListeningSocket *, SetupServer *> list_socket_server;
    std::vector<pollfd> pfds;
    int fd_count;
    int fd_size;
    struct Client
    {
        std::map<ListeningSocket *, SetupServer *>::iterator server;
        int fd_client;
        Client(std::map<ListeningSocket *, SetupServer *>::iterator server, int client);
    };
    std::vector<Client> connections;
    

public:
    Server(int domain, int service, int protocole, std::vector<SetupServer *> SetupServers, int timeout);
    // Server(int domain, int service, int protocol, int port, u_long interface, int bklg);
    void launch();
    ListeningSocket* get_Socket(std::map<ListeningSocket* , SetupServer*>::iterator it);
    static void handle_signal(int signum);
};

#endif // SERVER_HPP