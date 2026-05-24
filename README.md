# 42-Webserv
*This project has been created as part of the 42 curriculum by aghalmi, amoderan, asmati*
Webserv for 42


## Description

Webserv is an HTTP/1.1 server written in C++98. It handles GET, POST, and DELETE methods, supports CGI execution (Python, PHP), file uploads, directory listing, cookies and session management, and can serve multiple virtual servers from a single configuration file.

## Instructions

### Compilation
make

### Execution
./webserv [configuration file]

### Example
./webserv test.conf

## Ressources

-https://www.bureaudestalents.com/glossaire-tech/rfc

-https://developer.mozilla.org/fr/docs/Web/HTTP

-https://www.ibm.com/docs/fr/i/7.6.0?topic=services-telnet

-https://en.wikipedia.org/wiki/Common_Gateway_Interface

-https://blog.stephane-robert.info/docs/services/web/nginx/

https://mondedie.fr/d/9343-comment-bien-configurer-les-location-nginx

https://www.geeksforgeeks.org/cpp/socket-programming-in-cpp/

https://learn.microsoft.com/fr-fr/windows/win32/api/winsock/nf-winsock-setsockopt

https://hea-www.harvard.edu/~fine/Tech/addrinuse.html

https://openclassrooms.com/forum/sujet/reseau-fonction-accept-76969

https://stackoverflow.com/questions/54715562/what-does-sin-mean-in-sin-addr-sin-family-etc

https://stackoverflow.com/questions/16508685/understanding-inaddr-any-for-socket-programming

https://www.gta.ufrj.br/ensino/eel878/sockets/sockaddr_inman.html

https://learn.microsoft.com/fr-fr/windows/win32/api/winsock2/nf-winsock2-listen

https://man7.org/linux/man-pages/man2/poll.2.html

https://www.reddit.com/r/C_Programming/comments/eeqi8r/understanding_how_to_use_poll_in_socket/?tl=fr

https://developer.mozilla.org/fr/docs/Web/HTTP/Reference/Methods/GET

https://developer.mozilla.org/fr/docs/Web/HTTP/Reference/Methods/POST

https://developer.mozilla.org/fr/docs/Web/HTTP/Reference/Methods/DELETE


1. test du conf file
avec arg existant et non existant et sans

2. test basic (methode get)
curl http://localhost:8080
curl http://localhost:8080/index.html

3. test static website
curl http://localhost:8080/script.js
curl http://localhost:8080/style.css

4. test directory listening method uploads
curl http://localhost:8080/uploads/
methode post avec -X
curl -X POST http://localhost:8080/uploads/new.txt -d "hello"

5. test error page
curl http://localhost:8080/inexistant.html      404
curl -X PUT http://localhost:8080/              405

6. test methode delete
curl -X DELETE http://localhost:8080/uploads/test.txt

7. test cgi
curl http://localhost:8080/script.py

8. test multi port (multi serveur)
curl http://localhost:8080
curl http://localhost:8081 ajt de bloc dans test.conf

9. test coockie
curl -v http://localhost:8080 2>&1 | grep -i cookie
curl -v --cookie "session_id=17796212251681692777" http://localhost:8080 2>&1 | grep -i cookie
doit avoir le meme cookie pour verif si bien renvoyer

10. stress test
for i in $(seq 1 100); do curl -s http://localhost:8080/ > /dev/null; done && echo "OK - 100 requetes reussies"

11. test telnet
telnet localhost 8080
GET / HTTP/1.1
Host: localhost