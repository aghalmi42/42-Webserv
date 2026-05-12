
#include "../../includes/Response.hpp"


Response::Response(const std::string& raw, const Server& server)
    : _server(server)
{
    _parseRequest(raw);
}

Response::~Response() {}

Response &Response::operator=(const Response& src)
{
	if(this != &src)
	{
		_methode = src._methode;
		_path = src._path;
		_response = src._response;
		_server = src._server;
        _headers = src._headers;
        _version = src._version;
	}
	return (*this);
}

Response::Response(const Response& src)
{
	*this = src;
}

void Response::_parseRequest(const std::string& HttpRequest)
{
   std::string line;
   std::istringstream requestLine(HttpRequest);
    if (std::getline(requestLine, line))
    {
         std::istringstream lineStream(line);
         lineStream >> _methode >> _path >> _version;
    }
    while(std::getline(requestLine, line))
    {
        if (line == "\r")
            break ;
        size_t separator = line.find(":");
        std::string key = line.substr(0,separator);
        std::string value = line.substr(separator + 2);
        value = strTrim(value);
        _headers[key] = value;
    }
    
    while(std::getline(resquestLine,line))
    {
        if(requestLine.empty())
            return; 
        _body += line + "\n";
    }
}

std::string Response::_readFile(const std::string& filepath)
{
	std::ifstream file(filepath.c_str());
	if (!file.is_open())
   		 return ("");
	std::string content;
	std::string line;

	while(getline(file, line)){
		content += line + "\n";
	}
	return (content);
}

std::string Response::_CreateResponse(int code, const std::string& body)
{
    std::string status;
    if (code == 200) status = "200 OK";
    else if (code == 404) status = "404 Not Found";
    else if (code == 403) status = "403 Forbidden";
    else if (code == 405) status = "405 Method Not Allowed";
    else if (code == 500) status = "500 Internal Server Error";
    else status = "500 Internal Server Error";
    //add les autre code de status
    std::ostringstream res;
    res << "HTTP/1.1 " << status << "\r\n";
    res << "Content-Type: text/html\r\n";
    res << "Content-Length: " << body.size() << "\r\n";
    res << "\r\n";   // ligne vide obligatoire
    res << body;
    return (res.str());
}

std::string Response::_ErrorPageResponse(int code)
{

    if (_server.errorPage.count(code))
    {
        std::string filepath = _server.errorPage[code];
        std::string body = _readFile("www" + filepath);
        if (!body.empty())
            return (_CreateResponse(code, body));
    }
    std::ostringstream fallback;
    fallback << "<html><body><h1>" << code << "</h1></body></html>";
    return (_CreateResponse(code, fallback.str()));
}

void Response::build()
{
  //todo
}

std::string Response::get() const
{
    return (_response);
}
