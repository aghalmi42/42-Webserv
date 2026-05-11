
#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <iostream> // pr assembler les header http
#include <string>
#include <fstream> // pr ouvrir des fichier sur le disque
#include <sstream> // pour construire une string moreau par moreau
#include "ServerConfig.hpp"


class Response
{
	private:
		std::string _methode;
		std::string _path;
		std::string _response;
		Server		_server;

		void  _parseRequest(const std::string& raw);
		std::string _readFile(const std::string& file);
		std::string _CreateResponse(int code, const std::string& string);
		std::string _ErrorPageResponse(int code);

	public:
		Response(const std::string& request,const Server& config);
		Response(const Response& src);
		Response& operator=(const Response& src);
		~Response();

		void build();
		std::string get() const;

};

//std ostream a add jsais pu la commander aller chercher sur les ancien cpp



#endif
