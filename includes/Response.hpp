
#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "ServerConfig.hpp"
#include "utils.hpp"
#include <algorithm>
#include <unistd.h> 
#include <sys/wait.h>
#include <dirent.h>
#include <ctime>
#include <cstdlib>

class Response
{
	private:
		std::string _methode;
		std::string _path;
		std::string _response;
		std::string _version;
		std::string _body;
		std::string _cookie;
		std::string _executeCgi();
		Server		_server;
		Location 	_location;
		std::map<std::string, std::string> _headers;
		void  _parseRequest(const std::string& raw);
		std::string _readFile(const std::string& file);
		std::string _CreateResponse(int code, const std::string& string);
		std::string _ErrorPageResponse(int code);
		std::string _getContentType();
		std::string _generateSessionId();
		void _handleCookies();

	public:
		Response(const std::string& request,const Server& config,const Location& location);
		Response(const Response& src);
		Response& operator=(const Response& src);
		~Response();

		void build();
		int ArgsVerif();
		std::string get() const;

};

#endif
