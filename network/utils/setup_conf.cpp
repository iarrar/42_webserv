#include "setup_conf.hpp"

int	check_format(std::string server_config)
{
	int	i = 0;
	int gauche = 0;
	int droite = 0;
	while (server_config[i])
	{
		if (server_config[i] == '{')
		{
			gauche++;
			i++;
		}
		else if (server_config[i] == '}')
		{
			droite++;
			i++;
		}
		else
			i++;
	}
	if (gauche != droite)
	{
		std::cout << "Error: Invalid or incomplete scopes in configuration file" << std::endl;
		return (1);
	}
	std::cout << "Scopes ok !" << std::endl;
	return (0);
}

std::string	get_config(char *str)
{
    std::string config_string;
    std::string server_config;
    std::ifstream   config_file(str);

    if (!config_file.is_open())
    {
        std::cout << "Could not open the configuration file" << std::endl;
        //return "ERROR";
    }
    std::stringstream   buffer;
    std::string line;
    while (getline(config_file, line))
    {
        buffer << line << '\n';
        config_string = config_string + line;
    }
    std::string config_contents = buffer.str();
    config_file.close();
	server_config = config_contents;
    return (server_config);
}

std::vector<SetupServer *>	serversInit(std::string server_config)
{
	std::string     scope;
	std::string		scope_name = "server";
    int i = 0;
    std::vector<SetupServer *> Servers;
    size_t pos = server_config.find(scope_name, 0);
    while (pos != std::string::npos && i == 0)
    {
        pos = pos + scope_name.size();
        while ((server_config[pos] >= 9 && server_config[pos] <= 13) || server_config[pos] == 32)
            pos++;
        if (server_config[pos] != '{')
            i++;
        else
        {
            int l = 1;
            int r = 0;
            size_t debut = pos;
            while ((l - r != 0) || l == 0)
            {
                pos++;
                if (server_config[pos] == '{')
                    l++;
                if (server_config[pos] == '}')
                    r++;
            }
            scope = server_config.substr(debut, (pos - debut + 1));
			SetupServer *st = new SetupServer(scope);
			Servers.push_back(st);
        }
		pos = server_config.find(scope_name, pos);
    }
	return (Servers);
}

unsigned int	ipConverter(std::string ip_address)
{
	unsigned int	cup[4];
	size_t	i = 0;
	int		j = 0;
	int		error = 0;
	size_t	debut = 0;
	std::string	buffer;
    unsigned int    ip = 0;
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
    return (ip);
}

int set_ip_value(std::string value_name, std::string server_scope)
{
	
		std::string value_string;
		std::size_t i = server_scope.find(value_name, 0);
		if (i == std::string::npos)
			return (0);
		if (i != std::string::npos)
		{
			while (!(server_scope[i] >= '0' && server_scope[i] <= '9'))
				i++;
			while ((server_scope[i] >= '0' && server_scope[i] <= '9') || server_scope[i] == '.')
			{
                if (server_scope[i] == '.' && !(server_scope[i+1] >= '0' && server_scope[i + 1] <= '9'))
                {
                    std::cout << "\nInvalid ip adress\n" << std::endl;
                    return (0);
                }
                else
                {
                    value_string = value_string + server_scope[i];
                    i++;
                }
			}
			if (server_scope[i] != ';')
			{
				std::cout << " ';' Missing at the end line "<< value_name << std::endl;
				return (0);
			}
        }
		return (ipConverter(value_string));
}

std::vector<int>	setPorts(std::string server_scope, int *error)
{
	std::string	value_name = "listen";
		int value = 0;
		std::string value_string;
		std::size_t i = 0;
        std::vector <int> ports;
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

	if (ports.size() == 0)
	{
		std::cout << "Warning : no ports in the config file, please fix. Server set on port 1024 only" << std::endl;
		ports.push_back(1024);
		return ports;
	}
    return (ports);
}




bool        check_ip(std::string server_config)
{
    std::string     scope;
	std::string		scope_name = "server";
    int i = 0;
    //int j = 0;
    int index = 0;
    int error = 0;

    std::map<int, std::vector<int> > Servers;
    size_t pos = server_config.find(scope_name, 0);
    while (pos != std::string::npos && i == 0)
    {
        pos = pos + scope_name.size();
        while ((server_config[pos] >= 9 && server_config[pos] <= 13) || server_config[pos] == 32)
            pos++;
        if (server_config[pos] != '{')
            i++;
        else
        {
            int l = 1;
            int r = 0;
            size_t debut = pos;
            while ((l - r != 0) || l == 0)
            {
                pos++;
                if (server_config[pos] == '{')
                    l++;
                if (server_config[pos] == '}')
                    r++;
            }
            scope = server_config.substr(debut, (pos - debut + 1));
			
            Servers[index] = setPorts(scope, &error);
			Servers[index].push_back(set_ip_value("ip", scope));
        }
        index++;
		pos = server_config.find(scope_name, pos);
    }
    int test = Servers[0].back();
    std::map<int, std::vector<int> >::iterator it = Servers.begin();
    std::map<int, std::vector<int> >::iterator it_bis = Servers.begin();
    it_bis++;
    while (it != Servers.end())
    {
        test = it->second.back();
        while (it_bis != Servers.end())
        {
            if (it_bis->second.back() == test && (it->first != it_bis->first))
            {
                size_t s = 0;
                size_t t = 0;
                //while (it->second[s] != it->second.back())
                while (s != it->second.size() - 1)
                {
                    //while (it_bis->second[t] != it_bis->second.back())
                    while (t != it_bis->second.size() - 1)
                    {
                        if (it_bis->second[t] == it->second[s])
                            return (false);
                        else
                            t++;
                    }
                    t = 0;
                    s++;
                }
            }
            else
                it_bis++;
        }
        it_bis = Servers.begin();
        it++;
    }
    return (true);
}

bool    checkPorts(std::vector<int> vec1, std::vector<int> vec2)
{
    size_t i = 0;
    size_t j = 0;
    while (i != vec1.size())
    {
        while (j != vec2.size())
        {
            if (vec1[i] == vec2[j])
                return (false);
            else
                j++;
        }
        j = 0;
        i++;
    }
    return (true);
}

int get_config_value(std::string server_scope)
{
    int value = 0;
    std::string value_string;
    std::string value_name = "time_out";
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
            return (0);
        }
        value = atoi(value_string.c_str());
        return (value);
    }
    return (0);
}
