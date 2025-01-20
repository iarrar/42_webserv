#include <sys/types.h>


struct ServerConfig
{
	int 	port;
	int 	service;
	int		protocol;
	int		domain;
	int		backlog;
	u_long	interface;
};

ServerConfig*	config_moulinette(char *str);//ouvrir le fichier, choper la config et retourner la structure
