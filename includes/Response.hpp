
#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <iostream> // pr assembler les header http
#include <string>
#include <fstream> // pr ouvrir des fichier sur le disque
#include <sstream> // pour construire une string moreau par moreau
#include "ServerConfig.hpp"
#include "utils.hpp"
#include <algorithm>
#include <unistd.h> 
#include <sys/wait.h>
class Response
{
	private:
		std::string _methode;
		std::string _path;
		std::string _response;
		std::string _version;
		std::string _body;
		std::string _executeCgi();
		Server		_server;
		Location 	_location;
		std::map<std::string, std::string> _headers; //dictionare clef valeur ;

	//note a moi meme si je rajoute une class prive pas oublier de lajouter dans 
	// l'operateur de recopie 

		void  _parseRequest(const std::string& raw);
		std::string _readFile(const std::string& file);
		std::string _CreateResponse(int code, const std::string& string);
		std::string _ErrorPageResponse(int code);

	public:
		Response(const std::string& request,const Server& config,const Location& location);
		Response(const Response& src);
		Response& operator=(const Response& src);
		~Response();

		void build();
		int ArgsVerif();
		std::string get() const;

};

//std ostream a add jsais pu la commander aller chercher sur les ancien cpp

// GET /index.html HTTP/1.1\r\n
// Host: localhost:8080\r\n
// User-Agent: Mozilla/5.0\r\n
// Accept: text/html\r\n
// \r\n


// POST /upload HTTP/1.1\r\n
// Host: localhost:8080\r\n
// Content-Type: text/plain\r\n
// Content-Length: 13\r\n
// \r\n
// hello world !

#endif
