#ifndef BASEREQUEST_HPP
#define BASEREQUEST_HPP

#include "../utils/colors.h"
#include "../server/SetupServer.hpp"

#include <string>
#include <poll.h>
#include <map>
#include <vector>
#include <string>
#include <map>
#include <iterator>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


class BaseRequest
{
private:
    int nbytes;
    int client_fd;
    int listener_fd;
    static int cgi_pid;
    char buffer[30000];
	bool isBinary;
    std::string method;
    std::string path;
    std::string version;
    std::map<std::string, std::string> headers;
    std::string body;
    std::string statusCode;
    std::string pageContent;
    std::string response;
    std::string mimeType;
    std::size_t findCloseConnection;
    std::vector<char> binaryContent;
    std::map<std::string, std::string> accessRights;

    SetupServer *_configServer;

    void parseRequest(std::string request);
    void debugBuffer(const char *buf);
    void debugHeaders();
    void processRequest();
    static void timeout_handler_CGI(int signum);
    void reformatPath();
    void buildRedirectResponse();
	void buildBinaryResponse();
	void buildTextResponse();
	void sendBinaryResponse();
	void sendTextResponse();
	void get(std::string path);
	int	 post(std::string path);
    void del(std::string path);
    std::string getPage(std::string path);
    // const char* buildResponse();
    void handleAnyType(const std::string& body, std::string type);
    void handleCGI(std::string path, std::string method, char **methods);
    void handleBinaryData(const std::string& type);
    void handleMultipartData(const std::string& body, std::string content_type);
    void getMimeType();
    std::vector<char> readFile(const std::string &path, bool isBinary);
    std::vector<char> readBinaryFile(const std::string& path);
    void sendResponse();
    bool hasAccess(const std::string& path, const std::string& method);



    

public:
    BaseRequest(int sender_fd, int listener, SetupServer *configServer);
    int getNbytes() const;
    std::size_t getFindCloseConnection() const;
    std::size_t getFindCloseResponse() const;
    int getClientFd() const;
    void setStatusCode(int status);

};

#endif // BASEREQUEST