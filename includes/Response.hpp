
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
		std::string _version;
		std::string _body;
		Server		_server;
		std::map<std::string, std::string> _headers; //dictionare clef valeur ;

	//note a moi meme si je rajoute une class prive pas oublier de lajouter dans 
	// l'operateur de recopie 

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
