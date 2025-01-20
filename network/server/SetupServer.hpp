#ifndef SETUPSERVER_HPP
#define SETUPSERVER_HPP

#include "SimpleServer.hpp"
#include "../sockets/ListeningSocket.hpp"
#include <map>
#include <vector>
#include <sys/types.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <dirent.h>

class Root
{
	private :
		std::vector<std::string>	methodes;
		std::string             	route;
		std::string					default_file;
		std::string					default_path;
		bool						GET;
		bool						POST;
		bool						DELETE;
		bool						UNKNOWN;
		bool						default_scope; // 
		int							body_size;

	public :
		Root(std::string scope);
		// ~Root();
		bool		checkMethode(std::string methode) const;
		std::string	getRoot() const;
		std::string	getDefaultFile() const;
		int			 getBodySize() const;
		void		setBodySize(int body_size);
		void		setDefaultPath(std::string path);
		bool		getDefaultScope() const; 
		std::string	getDefaultPath() const;
		void		setDefaultScope(bool lol);


};

class SetupServer
{
	private :
				std::string					error_root;		        //chemin d'acces aux pages d'erreur
				// std::string					page_root;		        //chemin d'acces aux pages du site
				// std::string					root_location;	                //pas encore utilisee, pas sur que ce soit le cas un jour
				// std::vector<std::string>	methodes;
				std::string					server_name;//		        //server_name
				std::string					server_data;///	                //copie du scope du serveur
				std::string					ip_address;///                     //l'addresse d'ip_address en string
				std::vector<std::string> roots;    //Map root - liste des methodes autorisees ATTENTION !!! sera obsolete quand la structure root sera ok
				std::map<std::string, Root* > map_root;
				std::string					default_path;
				std::string					default_scope;
				int					time_out;//A mettre dans serveur//		//time_out du serveur, je sais pas encore a quoi il pourrait servir
				std::vector<int>	ports;///			//port de la socket du serveur, potentiellement a transformer en tableau d'int
				int					body_size;		//taille max du body client
				int					max_connections;//A mettre dans serveur//        //connection max sur le serveur
				int					error;///			//valeur initialisee a zero, est incrementee si une erreur est rencontree pendant le parsing
				in_addr_t			ip;///                     //l'addresse d'ip_address en in_addr_t
				void				getRoot(std::string location);
				int					get_config_value(std::string value_name, std::string server_scope);
				void				setPorts(std::string server_scope);
				void				set_ip_value(std::string value_name, std::string server_scope);
				void				ipConverter();
				void				setAllValues();
				void				setDefaultPath(std::string value_name);
				std::string			get_config_string(std::string value_name, std::string server_scope);
				std::map<std::string, std::string>	redirection;


	public :
				SetupServer(std::string server_scope);
				~SetupServer();
				// Getters
				void			findScope(std::string scope_name); 
				std::string	getDefaultScope(); //
				std::string getServerName() const { return this->server_name; }
				// std::string getErrorRoot() const { return this->error_root; }
				// std::string getPageRoot() const { return this->page_root; }
				// std::string getRootLocation() const { return this->root_location; }
				std::string getServerData() const { return this->server_data; }
				std::string getIpAddress() const { return this->ip_address; }
				// std::vector<std::string> getMethodes() const { return this->methodes; }
				// std::map<std::string, std::vector<std::string> > getAllowances() const { return this->allowances; }
				int getTimeOut() const { return this->time_out; }
				std::vector<int> getPort() const { return this->ports; }
				int getBodySize() const { return this->body_size; }
				int getMaxConnections() const { return this->max_connections; }
				int getError() const { return this->error; }
				in_addr_t getIp() const { return this->ip; }
				std::string	getDefaultPath() const {return (this->default_path);}
				std::map<std::string, Root*>& getPath() {return (this->map_root);}
				std::vector<std::string>&		getPathVector() {return (this->roots);}
				std::string				getErrorPath(){return (this->error_root);}
				std::string				getDefaultScopePath() const {return (this->default_scope);}
				bool					checkRedirection(std::string);
				std::string				getRedirection(std::string);
};

#endif