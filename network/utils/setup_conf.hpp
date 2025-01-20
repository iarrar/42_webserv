#ifndef SETUP_CONF_HPP
#define SETUP_CONF_HPP

#include <arpa/inet.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <vector>
#include <sys/types.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sys/types.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "../server/SetupServer.hpp"

int	check_format(std::string server_config);
std::string	get_config(char *str);
std::vector<SetupServer *>	serversInit(std::string server_config);
bool        check_ip(std::string server_config);
int get_config_value(std::string server_scope);
bool        checkPorts(std::vector<int> vec1, std::vector<int> vec2);

#endif