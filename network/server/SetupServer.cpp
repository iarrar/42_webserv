#include "SetupServer.hpp"

SetupServer::SetupServer(std::string server_scope)
{
    error = 0;
	setAllValues();
	server_data = server_scope;
	setPorts(server_scope);
	body_size =  get_config_value("client_max_body_size", server_scope);
	max_connections = get_config_value("max_connections", server_scope);
	time_out = get_config_value("time_out", server_scope);
	error_root = get_config_string("base_error_path", server_scope);
	set_ip_value("ip", server_scope);
	default_path = "./ressources/";
	findScope("location");
	// std::map<std::string, Root*>::iterator	it = map_root.begin();
	// int index  = 0; 
	default_scope = getDefaultScope();
}

void SetupServer::setAllValues()
{
	    server_name = "localhost";
        server_data = "";
        ip = INADDR_ANY;
        time_out = 300;             // Default timeout in seconds
        body_size = 1048576;        // Default max body size: 1MB
        max_connections = 100;      // Default max connections
        error = 0;                  // No error initially
}

int SetupServer::get_config_value(std::string value_name, std::string server_scope)
{
	if (error == 0)
	{
		int value = 0;
		std::string value_string;
		std::size_t i = server_scope.find(value_name, 0);
		if (i != std::string::npos)
		{
			while (!(server_scope[i] >= '0' && server_scope[i] <= '9'))
				i++;
			while ((server_scope[i] >= '0' && server_scope[i] <= '9'))
			{
				value_string = value_string + server_scope[i];
				i++;
			}
			if (server_scope[i] != ';')
			{
				std::cout << " ';' Missing at the end line "<< value_name << std::endl;
				error++;
				return (0);
			}
			value = atoi(value_string.c_str());
			return (value);
		}
	}
	else
	{
		error++;
		std::cout << "Error : Could not set " << value_name << std::endl;
		return (0);
	}
    return (0);
}

std::string SetupServer::get_config_string(std::string value_name, std::string server_scope)
{
	if (error == 0)
	{
		// int value = 0;
		std::string value_string = "";
		std::size_t i = server_scope.find(value_name, 0) + value_name.size();
		if (i != std::string::npos)
		{
			while (server_scope[i] == 32 || (server_scope[i] >= 9 && server_scope[i] <= 13))
				i++;
			while (server_scope[i] != ';' && server_scope[i] != '\n')
			{
				value_string = value_string + server_scope[i];
				i++;
			}
			if (server_scope[i] != ';')
			{
				std::cout << " ';' Missing at the end line "<< value_name << std::endl;
				error++;
				return ("");
			}
			
			return (value_string);
		}
		else
		{
			error++;
			std::cout << "Error : Could not set " << value_name << std::endl;
			return ("");
		}
	}
	else
	{
		error++;
		std::cout << "Error : Could not set " << value_name << std::endl;
		return ("");
	}
    return ("");
}

void SetupServer::findScope(std::string scope_name) //ok
{
    std::string     scope;
    size_t pos = server_data.find(scope_name, 0);
	size_t debut;

	while (pos != std::string::npos)
	{
		if (pos != std::string::npos)
		{
			while (server_data[pos] != '{')
				pos++;
			if (server_data[pos] != '{')
				return ;
			else
			{
				int l = 1;
				int r = 0;
				debut = pos;
				while ((l - r != 0) || l == 0)
				{
					pos++;
					if (server_data[pos] == '{')
						l++;
					if (server_data[pos] == '}')
						r++;
				}
				scope = server_data.substr(debut, (pos - debut + 1));
				getRoot(scope);
			}
		}
		pos = server_data.find(scope_name, pos);
	}
}

void		SetupServer::getRoot(std::string scope)
{
	size_t pos = scope.find("root");
	size_t debut = pos + 5;
	while (scope[pos] != ';')
		pos++;
	std::string route = scope.substr(debut, pos - debut);
	DIR *dir;
	std::string search;
	dir = opendir(route.c_str());
	if (dir == NULL)
	{
		error++;
		std::cout << "Invalid root in \n" << scope << std::endl;
	}
	if (error == 0)
	{
		Root *Route = new Root(scope);
		Route->setBodySize(body_size);
		Route->setDefaultPath(default_path);
		roots.push_back(Route->getRoot());
		search = "default_scope";
		pos = scope.find(search);
		if ( pos != std::string::npos)
		{
			debut = pos + search.size();
			std::string search_field;
			while (scope[pos] != '\n' && scope[pos] != ';')
				pos++;
			search_field = scope.substr(debut, pos - debut + 1);
			if (search_field.find("on;") != std::string::npos)
				Route->setDefaultScope(true);
			else
				Route->setDefaultScope(false);
		}
		map_root[Route->getRoot()] = Route;
	}
	else
	{
		std::cout << "Could not find root in " << scope << " pages" << std::endl;
		error++;
		closedir(dir);
		return;
	}
	closedir(dir);
	search = "redirect ";
	pos = scope.find(search);
	if (pos != std::string::npos)
	{
		debut = pos +  search.size();
		pos = debut;
		while (scope[pos] != '=')
			pos++;
		std::string chemin = scope.substr(debut, pos - debut - 1);
		debut = pos + 2;
		while (scope[pos] != ';')
			pos++;
		std::string chemin_r = scope.substr(debut, pos - debut - 1);
		redirection[chemin] = chemin_r;
	}
}

bool					SetupServer::checkRedirection(std::string path)
{
	if (redirection.find(path) != redirection.end())
		return (true);
	else
		return (false);
}
				
				
std::string				SetupServer::getRedirection(std::string path)
{
	std::map<std::string, std::string>::iterator it = redirection.find(path);
	return (it->second);
}


void	SetupServer::setPorts(std::string server_scope)
{
	std::string	value_name = "listen";
	if (error == 0)
	{
		int value = 0;
		std::string value_string;
		std::size_t i = 0;
		while (server_scope.find(value_name, i) != std::string::npos)
		{
			i = server_scope.find(value_name, i);
			if (i != std::string::npos)
			{
				while (!(server_scope[i] >= '0' && server_scope[i] <= '9'))
					i++;
				while ((server_scope[i] >= '0' && server_scope[i] <= '9'))
				{
					value_string = value_string + server_scope[i];
					i++;
				}
				if (server_scope[i] != ';')
				{
					std::cout << " ';' Missing at the end line "<< value_name << std::endl;
					error++;
				}
				value = atoi(value_string.c_str());
				ports.push_back(value);
                value_string = "";
			}
		}
	}
	else
	{
		error++;
		std::cout << "Error : Could not set " << value_name << std::endl;
	}

	if (ports.size() == 0)
	{
		std::cout << "Warning : no ports in the config file, please fix. Server set on port 1024 only" << std::endl;
		ports.push_back(1024);
		return ;
	}
}

void SetupServer::set_ip_value(std::string value_name, std::string server_scope)
{
	if (error == 0)
	{
		std::string value_string;
		std::size_t i = server_scope.find(value_name, 0);
		if (i == std::string::npos)
			return ;
		if (i != std::string::npos)
		{
			while (!(server_scope[i] >= '0' && server_scope[i] <= '9'))
				i++;
			while ((server_scope[i] >= '0' && server_scope[i] <= '9') || server_scope[i] == '.')
			{
				value_string = value_string + server_scope[i];
				i++;
			}
			if (server_scope[i] != ';')
			{
				std::cout << " ';' Missing at the end line "<< value_name << std::endl;
				error++;
			}
			ip_address = value_string;
			ipConverter();
			return ;
		}
	}
	else
	{
		error++;
		std::cout << "Error : Could not set " << value_name << std::endl;
	}
	error++;
}

std::string		SetupServer::getDefaultScope() 
{
	std::map<std::string, Root*>::iterator	it = map_root.begin();
	while (it != map_root.end())
	{
		if (it->second->getDefaultScope())
		{
			return (it->second->getRoot() + it->second->getDefaultFile());
		}
		else
			it++;
	}
	error++;
	return ("");
}

void	SetupServer::ipConverter()
{
	unsigned int	cup[4];
	size_t	i = 0;
	int		j = 0;
	int		error = 0;
	size_t	debut = 0;
	std::string	buffer;
	while (ip_address[i] != 0 && error == 0)
	{
		while (ip_address[i] != '.' && ip_address[i] != '\0')
			i++;
		buffer = ip_address.substr(debut, (i - debut));
		cup[j] = (unsigned int)atoi(buffer.c_str());
		if (cup[j] > 255)
		{
			std::cout << "IP address error " << ip_address << std::endl;
			error++;
		}
		if (ip_address[i] != '\0')
			i++;
		j++;
		debut = i;
	}
	ip = (cup[0] << 24) | (cup[1] << 16) | (cup[2] << 8) | cup[3];
}

// ROOT FUNCTIONS

Root::Root(std::string scope)
{
	size_t pos = scope.find("root");
	size_t debut = pos + 5;
	std::string search;
	search = "default_file";
	while (scope[pos] != ';')
		pos++;
	route = scope.substr(debut, pos - debut);
	GET = false;
	DELETE = false;
	POST = false;
	UNKNOWN = false;
	default_file = "";
	default_scope = false;
	pos = scope.find(search);
	if (pos != std::string::npos)
	{
		pos = pos + search.size();
		debut = pos;
		while (scope[debut] == 32)
		{
			debut++;
			pos++;
		}
		while (scope[pos] != ';')
			pos++;
		default_file = scope.substr(debut, pos - debut);
	}
	if (scope.find("limit_except") != std::string::npos)
	{
		pos = scope.find("limit_except") + 12;
		if (scope.find("GET", pos) != std::string::npos)
			GET = true;
		if (scope.find("POST", pos) != std::string::npos)
			POST = true;
		if (scope.find("DELETE", pos) != std::string::npos)
			DELETE = true;
	}
	else
	{
		GET = true;
		POST = false;
		DELETE = false;
	}
	return;
}

bool		Root::getDefaultScope() const
{
	return (this->default_scope);
}

bool		Root::checkMethode(std::string methode) const
{
	if (methode == "GET")
		return (GET);
	else if (methode == "POST")
		return (POST);
	else if (methode == "DELETE")
		return (DELETE);
	else
		return (UNKNOWN);
}

std::string	Root::getRoot() const
{
	return (this->route);
}

std::string	Root::getDefaultFile() const
{
	return (this->default_file);
}

int			Root::getBodySize() const
{
	return (this->body_size);
}

void		Root::setBodySize(int body)
{
	this->body_size = body;
}

void		Root::setDefaultPath(std::string path)
{
	this->default_path = path;
}

std::string	Root::getDefaultPath() const
{
	return (this->default_path);
}

void		SetupServer::setDefaultPath(std::string value_name)
{
	if (error == 0)
	{
		std::string value_string = "";
		std::size_t i = server_data.find(value_name, 0);
		if (i != std::string::npos)
		{
			i = i + value_name.size();
			while(server_data[i] == 32 || (server_data[i] >= 9 && server_data[i] <= 13))
				i++;			
			while ((server_data[i] != ';'))
			{
				value_string = value_string + server_data[i];
				i++;
			}
			if (server_data[i] != ';')
			{
				std::cout << " ';' Missing at the end line "<< value_name << std::endl;
				error++;
				return ;
			}
			default_path = value_string;
			return ;
		}
	}
	else
	{
		error++;
		std::cout << "Error : Could not set " << value_name << std::endl;
		return;
	}
    return;
}

void		Root::setDefaultScope(bool lol)
{
	this->default_scope = lol;
}

SetupServer::~SetupServer()
{
	std::map<std::string, Root*>::iterator it = map_root.begin();
	while (it != map_root.end())
	{
		delete it->second;
		it++;
	}
}