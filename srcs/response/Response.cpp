
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
    if (code == 200) status = "200 OK";
	else if (code == 201) status = "201 Created";
	else if (code == 204) status = "204 No Content";
	else if (code == 301) status = "301 Moved Permanently";
	else if (code == 400) status = "400 Bad Request";
    else if (code == 403) status = "403 Forbidden";
    else if (code == 404) status = "404 Not Found";
    else if (code == 405) status = "405 Method Not Allowed";
	else if (code == 413) status = "413 Content Too Large";
    else if (code == 500) status = "500 Internal Server Error";
	else if (code == 505) status = "505 HTTP Version Not Supported";
    else status = "500 Internal Server Error";
    std::ostringstream res;
    res << "HTTP/1.1 " << status << "\r\n";
    res << "Content-Type: " << _getContentType() << "\r\n";
    res << "Content-Length: " << body.size() << "\r\n";
	if (!_cookie.empty())
    	res << "Set-Cookie: " << _cookie << "\r\n";
    res << "\r\n";
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

std::string Response::_executeCgi()
{
	int pipefd[2];
	if(pipe(pipefd) == -1)
	{
		std::cerr << "Error: pipe failed" << std::endl;
		return ("");
	}
	char buff[4096];
	pid_t pid = fork();
	if (pid == -1)
	{
		std::cerr << "Error: fork failed" << std::endl;
		return ("");
	}
	if(pid == 0)
	{
		std::string interpreter;
		std::string ext = _path.substr(_path.find_last_of("."));
		if (ext == ".py")
    		interpreter = "/usr/bin/python3";
		else if (ext == ".php")
    		interpreter = "/usr/bin/php-cgi";
		else
    		interpreter = "/usr/bin/python3";
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		std::string scriptPath = _location.root + _path;
		char *arg[3];
		arg[0] = (char *)interpreter.c_str();
		arg[1] = (char *)scriptPath.c_str();
		arg[2] = NULL;
		execve(interpreter.c_str(), arg, NULL);
		exit (1);
	}
	else
	{
    	close(pipefd[1]);
    	int status;
    	int waited = 0;
    	while (waited < 5)
    	{
        	if (waitpid(pid, &status, WNOHANG) != 0)
            	break;
        	sleep(1);
        	waited++;
    	}
    	if (waited >= 5)
    	{
        	kill(pid, SIGKILL);
        	waitpid(pid, &status, 0);
    	}
    	ssize_t octets = read(pipefd[0], buff, sizeof(buff));
    	close(pipefd[0]);
    	std::string output = std::string(buff, octets);
    	size_t sep = output.find("\r\n\r\n");
    	if (sep != std::string::npos)
       		return output.substr(sep + 4);
    	return output;
	}
}

void Response::build()
{
	_handleCookies();
	if(ArgsVerif() == 1)
		return;
	if (!_location.redirection.empty())
	{
    	std::ostringstream res;
    	res << "HTTP/1.1 301 Moved Permanently\r\n";
    	res << "Location: " << _location.redirection << "\r\n";
    	res << "Content-Length: 0\r\n";
    	res << "\r\n";
    	_response = res.str();
    	return;
	}
	size_t dotpos = _path.find_last_of(".");
	if(dotpos != std::string::npos && !_location.extensionCgi.empty() && _path.substr(dotpos) == _location.extensionCgi)
	{
		_response = _CreateResponse(200, _executeCgi());
		return ;
	}
	if(_methode == "GET")
	{
		if (_path == "/")
			_path = "/" + _location.fileIndex;
		std::string fullPath = _location.root + _path;
		DIR *dir = opendir(fullPath.c_str());
		if (dir != NULL && _location.autoIndex)
		{
			std::ostringstream html;
			html << "<html><body><h1>Index of " << _path << "</h1><ul>";
			struct dirent *entry;
			while ((entry = readdir(dir)) != NULL)
			{
    			std::string name = entry->d_name;
				if (name == "." || name == "..")
        			continue;
    			html << "<li><a href='" << _path << name << "'>" << name << "</a></li>";
			}
			html << "</ul></body></html>";
			_response = _CreateResponse(200, html.str());
			closedir(dir);
			return;
		}
		std::string body = _readFile(_location.root + _path);
		if(access((_location.root + _path).c_str(), F_OK) == 0)
    		_response = _CreateResponse(200, body);
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
			_response = _CreateResponse(201, "");
		}
	}
	else if(_methode == "DELETE")
	{
		std::string FileName = _path.substr(_path.find_last_of("/") + 1);
		std::string FilePath = _location.root + _path; 
		if(remove(FilePath.c_str()) == 0)
			_response = _CreateResponse(204, "");
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

std::string Response::_getContentType()
{
    size_t dot = _path.find_last_of(".");
    if (dot == std::string::npos)
        return "text/html";
    std::string ext = _path.substr(dot);
    if (ext == ".html") return "text/html";
    if (ext == ".css")  return "text/css";
    if (ext == ".js")   return "application/javascript";
    if (ext == ".png")  return "image/png";
    if (ext == ".jpg")  return "image/jpeg";
    if (ext == ".ico")  return "image/x-icon";
	if (ext == ".py") return "text/html";
	if (ext == ".php") return "text/html";
    return "text/plain";
}

std::string Response::_generateSessionId()
{
    std::ostringstream ss;
    ss << time(NULL) << rand();
    return (ss.str());
}

void Response::_handleCookies()
{
    if (_headers.count("Cookie"))
        _cookie = _headers["Cookie"];
    else
        _cookie = "session_id=" + _generateSessionId();
}
