#include "BaseRequest.hpp"
#include "../utils/utils.hpp"
#include "../server/SetupServer.hpp"

BaseRequest::BaseRequest(int sender_fd, int listener, SetupServer *configServer) : client_fd(sender_fd), listener_fd(listener), _configServer(configServer)
{
    std::string request;
	this->isBinary = false;
    nbytes = recv(client_fd, buffer, sizeof(buffer), 0);
    if (nbytes <= 0)
    {
        if (nbytes == -1)
        {
            setStatusCode(400);
        }
        return ;
    }
    request.append(buffer, nbytes);
    std::size_t headers_end = request.find("\r\n\r\n");
    if (headers_end != std::string::npos)
    {
        std::size_t body_start = headers_end + 4;
        std::size_t content_length = 0;
        std::size_t content_length_pos = request.find("Content-Length:");
        if (content_length_pos != std::string::npos)
        {
            content_length = atoi(request.substr(content_length_pos + 16).c_str());
        }
        std::size_t body_received = request.size() - body_start;
        if ((size_t)_configServer->getBodySize() > content_length)
        {
            while (body_received < content_length)
            {
                nbytes = recv(client_fd, buffer, sizeof(buffer), 0);
                if (nbytes <= 0)
                {
                    if (nbytes == -1)
                    {
                        setStatusCode(400);
                    }
                    return ;
                }
                request.append(buffer, nbytes);
                body_received += nbytes;
            }
            debugBuffer(request.c_str());
            parseRequest(request);
            processRequest();
        }
        else
            setStatusCode(413);
        if (_configServer->checkRedirection(this->path))
        {
            setStatusCode(301);
        }
        if (this->statusCode == "301 Moved Permanently" || this->statusCode == "302 Found")
        {
            buildRedirectResponse();
            sendTextResponse();
        }
        else
        {
            this->isBinary ? buildBinaryResponse() : buildTextResponse();
            this->isBinary ? sendBinaryResponse() : sendTextResponse();
        }
        findCloseConnection = request.find("Connection: close");
        // break ;
    }
}

void BaseRequest::debugBuffer(const char *buf)
{
	int i = 0;
	while (buf[i])
	{
		if (buf[i] == '\r')
			std::cout << RED << "/r" << WHITE;
		else if (buf[i] == '\n')
			std::cout << RED << "/n" << WHITE << std::endl;
		else
			std::cout << ORANGE << buf[i] << WHITE;
		i++;
	}
}

void BaseRequest::debugHeaders()
{
    std::cout << ORANGE << "\n******************************************" << WHITE << std::endl;
    std::cout << "Method: " << this->method << std::endl;
    std::cout << "Path: " << this->path << std::endl;
    std::cout << "Version: " << this->version << std::endl;
	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
	{
		std::cout << BLUE << it->first << WHITE << " = " << it->second << std::endl;
	}
    std::cout << ORANGE << "******************************************\n" << WHITE << std::endl;
}

void BaseRequest::parseRequest(std::string request)
{
    std::istringstream requestStream(request);
    std::string line;

    // Getting first line of the request
    if (std::getline(requestStream, line))
	{
        std::istringstream lineStream(line);
        lineStream >> this->method;
        lineStream >> this->path;
        lineStream >> this->version;
		reformatPath(); // Changes request path to match working directory
    }
	// Getting headers
    while (std::getline(requestStream, line) && line != "\r")
	{
        std::string key;
        std::string value;
        std::istringstream headerStream(line);
        
        if (std::getline(headerStream, key, ':') && std::getline(headerStream, value))
		{
            // Cleaning spaces
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));

            this->headers[key] = value;
        }
    }
    // BODY request if present
    if (requestStream)
	{
        std::string body;
        while (std::getline(requestStream, line))
		{
            body += line + "\n";
        }
        this->body = body;
        // this->body = getRideOfPlus(this->body);
    }
}

void BaseRequest::buildRedirectResponse()
{
    std::string httpVersion = "1.1";
    // Construire l'en-tête HTTP
    this->response = "HTTP/" + httpVersion + " " + this->statusCode + "\r\n";
    this->response += "Location: " + _configServer->getRedirection(this->path) + "\r\n";
    this->response += "Content-Length: 0\r\n";  // Pas de corps dans la réponse
    this->response += "Connection: close\r\n"; // Fermer la connexion (optionnel)
    this->response += "\r\n";  // Fin des en-têtes
    std::cout << RED << this->response << WHITE << std::endl;
}

bool BaseRequest::hasAccess(const std::string& path, const std::string& method)
{
    // Check prefixes
    // for (std::map<std::string, std::string>::iterator it = this->accessRights.begin();
        //  it != this->accessRights.end(); ++it)
	std::map<std::string, Root*>::iterator it;
	for (it = this->_configServer->getPath().begin(); it != this->_configServer->getPath().end(); ++it)
    {
        std::string prefix = it->first;
        if (path.find(prefix) == 0) // Path found
        {
            // const std::string& allowedMethods = it->second;
			if (path.find("../") != std::string::npos)
				return false;
			return (it->second->checkMethode(method));
            // Is the method allowed ?
        //     if (allowedMethods.find(method) != std::string::npos)
        //     {
        //         return true;
        //     }
        //     else
        //     {
        //         return false;
        //     }
        }
    }
	// Path not found in rules, qui ne dit mot consent (accès non précisé = tous les accès)
    setStatusCode(404);
    return true;
}

void BaseRequest::processRequest()
{
	// TO DELETE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// this->accessRights["ressources/documents/"] = "POST, DELETE";
	// this->accessRights["ressources/pages/"] = "GET, POST, DELETE";
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    if (!hasAccess(this->path, this->method) || this->statusCode == "403 Forbidden")
    {
        setStatusCode(403);
        perror("Error: Access denied"); // check perror
        std::cout << RED << "ACCESS DENIED" << WHITE << std::endl; // check debug
        return;
    }
	else if (this->statusCode == "404 Not Found")
    {
        // 404PATH
        try
		{
			this->pageContent = getPage(_configServer->getErrorPath());
		}
		catch (const std::exception& e)
		{
			std::cerr << "404 default page not found: " << e.what() << std::endl;
			this->pageContent = "";
		}
        this->binaryContent = readFile(_configServer->getErrorPath(), false);
        this->mimeType = "text/html";
		return;
    }
    // Check for access rights
	// Process request
    if (this->mimeType.find("text/") != std::string::npos)
		this->isBinary = true;
    if (this->method == "GET")
	{
		// if (_configServer->)
        get(this->path);
	}
	else if (this->method == "POST")
		post(this->path);
	else if (this->method == "DELETE")
		del(this->path);
	else
		setStatusCode(405);
}

void	BaseRequest::reformatPath() // 
{
	this->path.erase(0, 1); // Remove '/' from path
	if (path == "")
	{
		this->path = _configServer->getDefaultScopePath(); 	// this->path = "/index.html"; //Redirect ?
        return;
	}
	// else (this->path.substr(this->path.find_last_of(".") + 1) == "html")
	else
		this->path = _configServer->getDefaultPath() + this->path;
    std::map<std::string, Root*> map = _configServer->getPath();
	if (map.find(this->path) != map.end())
	{
		if (path.find_last_of("/") == (path.size() - 1) && map[path]->getDefaultFile() == "" && map[path]->checkMethode("POST") == false) /////////////////////////////////
		{
			setStatusCode(403);
		}
		else if (path.find_last_of("/") == (path.size() - 1) && map[path]->getDefaultFile() != "" && map[path]->checkMethode("POST") == false)
		{
			path = path + _configServer->getPath()[path]->getDefaultFile();
		}
	}
}

void BaseRequest::buildBinaryResponse()
{
    std::string httpVersion = "1.1";
    
    std::stringstream ss;
    ss << this->binaryContent.size();  // Calculer la longueur du contenu binaire
    std::string contentLength = ss.str();

    // Construire l'en-tête HTTP
    this->response = "HTTP/" + httpVersion + " " + this->statusCode + "\r\n";
    this->response += "Content-Type: " + this->mimeType + "\r\n";
    this->response += "Content-Length: " + contentLength + "\r\n";
    this->response += "Connection: keep-alive\r\n";
    this->response += "\r\n"; // Séparation entre en-tête et corps
}

void BaseRequest::buildTextResponse()
{
    std::string httpVersion = "1.1";
    std::string charset = "UTF-8";

    std::stringstream ss;
    ss << this->pageContent.length();  // Calculer la longueur du contenu texte
    std::string contentLength = ss.str();

    // Construire l'en-tête HTTP
    this->response = "HTTP/" + httpVersion + " " + this->statusCode + "\r\n";
    this->response += "Content-Type: " + this->mimeType + "; charset=" + charset + "\r\n";
    this->response += "Content-Length: " + contentLength + "\r\n";
    this->response += "Connection: keep-alive\r\n";
    this->response += "\r\n"; // Séparation entre en-tête et corps
    this->response += this->pageContent;  // Ajouter le contenu texte
}

void BaseRequest::sendBinaryResponse()
{

    size_t headerBytesSent = 0;
    size_t headerSize = this->response.length();
    const char* headerPtr = this->response.c_str();

    // Envoi de l'en-tête HTTP
    while (headerBytesSent < headerSize)
	{
        ssize_t bytesSent = send(this->client_fd, headerPtr + headerBytesSent, headerSize - headerBytesSent, 0);
        if (bytesSent <= 0)
		{
            if (bytesSent == 0)
            {
                std::cout << "Aucune donne envoye" << std::endl;
                std::cout << bytesSent << std::endl;
            }
            else
            {
                std::cerr << "Erreur lors de l'envoi de l'en-tête HTTP ou la connexion est fermée" << std::endl;
                std::cout << bytesSent << std::endl;
            }
            return;
        }
        headerBytesSent += bytesSent;
    }

    // Envoi du contenu binaire en paquets
    const char* dataPtr = this->binaryContent.data();
    size_t totalBytesSent = 0;
    size_t dataSize = this->binaryContent.size();
    size_t packetSize = 4096; // Taille du paquet en octets

    while (totalBytesSent < dataSize)
	{
		// usleep(1);
        size_t bytesToSend = std::min(packetSize, dataSize - totalBytesSent);
        ssize_t bytesSent = send(this->client_fd, dataPtr + totalBytesSent, bytesToSend, 0);
        if (bytesSent == -1)
		{
            std::cerr << "Erreur lors de l'envoi des données binaires ou la connexion est fermée" << std::endl;
            //return ;
        }
        else if (bytesSent == 0)
		{
            std::cout << "WARNING : Aucune donne envoyer" << std::endl;
            //return;
        }
        totalBytesSent += bytesSent;
    }
}

void BaseRequest::sendTextResponse()
{

    size_t contentBytesSent = 0;
    size_t contentSize = this->response.length();
    const char* contentPtr = this->response.c_str();

    // Envoi du contenu texte en boucle
    while (contentBytesSent < contentSize)
	{
        ssize_t bytesSent = send(this->client_fd, contentPtr + contentBytesSent, contentSize - contentBytesSent, 0);
        if (bytesSent <= 0)
        {
            if (bytesSent == 0)
            {
                std::cout << "Aucune donne envoye" << std::endl;
                std::cout << bytesSent << std::endl;
            }
            else
            {
                std::cerr << "Erreur lors de l'envoi de l'en-tête HTTP ou la connexion est fermée" << std::endl;
                std::cout << bytesSent << std::endl;
            }
        }
        return;
        contentBytesSent += bytesSent;
    }
}

int BaseRequest::getNbytes() const
{
    return(this->nbytes);
}

std::size_t BaseRequest::getFindCloseConnection() const
{
    return(this->findCloseConnection);
}

std::size_t BaseRequest::getFindCloseResponse() const
{
	return(this->response.find("Connection: close"));
}

int BaseRequest::getClientFd() const
{
	return(this->client_fd);
}


// METTRE LES PAGES D'ERREUR AU BON ENDROIT
// IMAGES/ TELECHARGE PAS BON