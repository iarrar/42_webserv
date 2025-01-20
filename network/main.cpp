#include "./server/Server.hpp"
#include "./server/SetupServer.hpp"
#include "./utils/setup_conf.hpp"
#include <arpa/inet.h>
#include <fstream>
#include <sstream>
#include <iostream>

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		if (argc < 2)
			std::cout << "Error : no config file specified" << std::endl;
		else
			std::cout << "Error : too much config file specified" << std::endl;
		return (1);
	}
	std::string server_config = get_config(argv[1]);	//fichier de configuration converti en std::String	
	if (check_format(server_config) == 1)
		return (1);
	std::vector<SetupServer *>	SetupServers = serversInit(server_config);		//tableau de serveur;
	size_t i = 0;
	size_t j = 0;
	in_addr_t ip = 0;
	while (i != SetupServers.size())
	{
		ip = SetupServers[i]->getIp();
		while (j != SetupServers.size())
		{
			if (ip == SetupServers[j]->getIp() && j != i)
			{
				if (!checkPorts(SetupServers[i]->getPort(), SetupServers[j]->getPort()))
				{
					std::cout << "Paires IP/Port identiques dans differents serveurs - Error\n";
					size_t k = 0;
					while (k != SetupServers.size())
					{
						delete SetupServers[k];
						k++;
					}
					return (0);
				}
			}
			j++;
		}
		j = 0;
		i++;
	}
	int timeout = get_config_value(server_config);
	Server main_server(AF_INET, SOCK_STREAM, 0, SetupServers, timeout);
	size_t k = 0;
	while (k != SetupServers.size())
	{
		delete SetupServers[k];
		k++;
	}
}