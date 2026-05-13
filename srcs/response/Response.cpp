
#include "../../includes/Response.hpp"


Response::Response(const std::string& raw, const Server& server, const Location& location)
    : _server(server), _location(location)
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
		_body = src._body;
		_location = src._location;
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
    
    while(std::getline(requestLine,line))
    {
        if(line.empty())
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
    if (code == 200) status = "200 OK";// new
	else if (code == 201) status = "201 Created";// new
	else if (code == 204) status = "204 No Content"; // new
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
		std::cerr << "repertoy" << "www" + filepath << std::endl;
        if (!body.empty())
            return (_CreateResponse(code, body));
    }
    std::ostringstream fallback;
    fallback << "<html><body><h1>" << code << "</h1></body></html>";
    return (_CreateResponse(code, fallback.str()));
}

int Response::ArgsVerif()
{
	std::vector<std::string>::iterator it = std::find
	(_location.methodHttp.begin(),
	_location.methodHttp.end(),
	_methode);
	if(_path.empty() || _methode.empty() || _version.empty())
	{
		_response = _ErrorPageResponse(400);
		return 1;
	}
	if(it == _location.methodHttp.end())
	{
		_response = _ErrorPageResponse(405);
		return 1;
	}
	if(_version != "HTTP/1.1")
	{
		_response = _ErrorPageResponse(505);
		return 1;
	}
	
	return 0;
}
void Response::build()
{
	if(ArgsVerif() == 1)
		return;	
	
	if(_methode == "GET")
	{
		if (_path == "/")
  		  _path = "/" + _location.fileIndex;
		//pr test // std::cerr << "chemin: " << _location.root + _path << std::endl;

		std::string body = _readFile(_location.root + _path);
		if(!body.empty())
			_response = _CreateResponse(200,body);
		else
			_response = _ErrorPageResponse(404);
	}
	else if(_methode == "POST")
	{
		if((long)_body.size() > _server.MaxSizeClient)
		{
			_response = _ErrorPageResponse(413);
			return ;
		}
		std::string fileName = _path.substr(_path.find_last_of("/") + 1);
		std::ofstream outfile(_location.uploadPath + "/" + fileName);
		
		if(!outfile.is_open())
			_response = _ErrorPageResponse(404);
		else
		{
			outfile << _body;
			_response = _CreateResponse(201 ,"");
		}
	}
	else if(_methode == "DELETE")
	{
		std::string FileName = _path.substr(_path.find_last_of("/") + 1);
		std::string FilePath = _location.root + _path; 
		if(remove(FilePath.c_str()) == 0)
			_response = _CreateResponse(204 ,"");
		else
			_response = _ErrorPageResponse(404);
	}
	else
		_response = _ErrorPageResponse(405);
}

std::string Response::get() const
{
    return (_response);
}
