# 42-Webserv

*Project completed as part of the 42 curriculum by aghalmi, amoderan, asmati*

An HTTP/1.1 server written in **C++98**, from scratch, with no external library.

## Description

Webserv is a non-blocking HTTP/1.1 server. A single, single-threaded process
handles every connection at once through `poll()`: no read/write operation ever
blocks the event loop. The server reads an nginx-style configuration file and can
expose several `server` blocks on different ports and `server_name` values.

### Features

| Area               | Details                                                              |
|--------------------|--------------------------------------------------------------------- |
| HTTP methods       | `GET`, `POST`, `DELETE` (allowed per `location`)                    |
| CGI                | Runs Python and PHP scripts based on the file extension             |
| Upload             | Stores files via `POST` into a configurable directory              |
| Directory listing  | `autoindex on` generates the listing of a directory               |
| Index file         | `index` served automatically for a directory                       |
| Error pages        | Custom pages per status code (`error_page`)                        |
| Body limit         | `client_max_body_size` → `413` when exceeded                       |
| Cookies / sessions | Assigns a `session_id` and tracks it across requests              |
| Multi-server       | Multiple `server` blocks, multiple ports, resolved by `Host`       |

## Requirements

- A C++ compiler with C++98 support (`c++` / `g++` / `clang++`)
- `make`
- Python 3 and/or PHP-CGI if you want to test CGI scripts

## Build

```
make
```

| Target        | Effect                                       |
|---------------|----------------------------------------------|
| `make`        | Builds the `webserv` binary                  |
| `make clean`  | Removes object files                         |
| `make fclean` | Removes object files and the binary          |
| `make re`     | `fclean` then a full rebuild                 |

Built with `-Wall -Wextra -Werror -std=c++98`.

## Usage

```
./webserv [configuration file]
```

Example:

```
./webserv test.conf
```

If no file is given, a default configuration is used.
The server runs in the foreground; press `Ctrl-C` to stop it.

## Configuration

The file follows nginx syntax: `server { ... }` blocks each containing one or
more `location { ... }` blocks. Every directive ends with `;`.

### `server` directives

| Directive                  | Purpose                                        |
|----------------------------|------------------------------------------------|
| `listen`                   | Listening port                                 |
| `server_name`              | Host name bound to the block                    |
| `client_max_body_size`     | Maximum request body size (bytes)              |
| `error_page <code> <path>` | Page returned for a given error code          |

### `location` directives

| Directive        | Purpose                                                |
|------------------|--------------------------------------------------------|
| `methods`        | HTTP methods allowed on this route                     |
| `root`           | Root directory served                                  |
| `index`          | Default file served for a directory                    |
| `autoindex`      | `on` / `off` — automatic directory listing             |
| `uploadPath`     | Destination directory for uploads                      |
| `extensionCgi`   | Extension that triggers CGI execution (`.py`, `.php`)  |

### Minimal example

```nginx
server {
    listen 8080;
    server_name localhost;
    client_max_body_size 1024;
    error_page 404 /404.html;

    location / {
        methods GET POST DELETE;
        root ./www;
        autoindex on;
        index index.html;
        uploadPath ./www/uploads;
        extensionCgi .py;
    }

    location /script.php {
        methods GET;
        root ./www;
        extensionCgi .php;
    }
}
```

See [test.conf](test.conf) for a two-server example.

## Project layout

```
srcs/
  main.cpp
  network/     ServerNetwork.cpp   – sockets, poll(), event loop
  parsing/     Parser.cpp          – reads and validates the config file
  http/        Routing.cpp         – location / route resolution
  response/    Response.cpp        – response building, CGI, upload
  utils/       utils.cpp           – helpers
includes/                          – headers
www/                               – test web root (pages, CGI, uploads)
```

## Quick tests

```sh
# 1. Config file: with a valid arg, an invalid arg, and no arg

# 2. Basic GET
curl http://localhost:8080
curl http://localhost:8080/index.html

# 3. Static website
curl http://localhost:8080/script.js
curl http://localhost:8080/style.css

# 4. Directory listing + upload
curl http://localhost:8080/uploads/
curl -X POST http://localhost:8080/uploads/new.txt -d "hello"

# 5. Error pages
curl http://localhost:8080/inexistant.html      # 404
curl -X PUT http://localhost:8080/              # 405

# 6. DELETE
curl -X DELETE http://localhost:8080/uploads/test.txt

# 7. CGI
curl http://localhost:8080/script.py

# 8. Multi-server / multi-port
curl http://localhost:8080
curl http://localhost:8081

# 9. Cookies / sessions
curl -v http://localhost:8080 2>&1 | grep -i cookie
curl -v --cookie "session_id=17796212251681692777" http://localhost:8080 2>&1 | grep -i cookie
# the returned cookie must be identical -> session kept correctly

# 10. Stress test
for i in $(seq 1 100); do curl -s http://localhost:8080/ > /dev/null; done && echo "OK - 100 requests succeeded"

# 11. Telnet (manual HTTP request)
telnet localhost 8080
GET / HTTP/1.1
Host: localhost
```

## Resources

**HTTP / RFC**

- https://www.bureaudestalents.com/glossaire-tech/rfc
- https://developer.mozilla.org/fr/docs/Web/HTTP
- https://developer.mozilla.org/fr/docs/Web/HTTP/Reference/Methods/GET
- https://developer.mozilla.org/fr/docs/Web/HTTP/Reference/Methods/POST
- https://developer.mozilla.org/fr/docs/Web/HTTP/Reference/Methods/DELETE
- https://en.wikipedia.org/wiki/Common_Gateway_Interface

**nginx configuration**

- https://blog.stephane-robert.info/docs/services/web/nginx/
- https://mondedie.fr/d/9343-comment-bien-configurer-les-location-nginx

**Sockets / networking**

- https://www.geeksforgeeks.org/cpp/socket-programming-in-cpp/
- https://www.gta.ufrj.br/ensino/eel878/sockets/sockaddr_inman.html
- https://learn.microsoft.com/fr-fr/windows/win32/api/winsock/nf-winsock-setsockopt
- https://learn.microsoft.com/fr-fr/windows/win32/api/winsock2/nf-winsock2-listen
- https://hea-www.harvard.edu/~fine/Tech/addrinuse.html
- https://openclassrooms.com/forum/sujet/reseau-fonction-accept-76969
- https://stackoverflow.com/questions/54715562/what-does-sin-mean-in-sin-addr-sin-family-etc
- https://stackoverflow.com/questions/16508685/understanding-inaddr-any-for-socket-programming

**poll() / multiplexing**

- https://man7.org/linux/man-pages/man2/poll.2.html
- https://www.reddit.com/r/C_Programming/comments/eeqi8r/understanding_how_to_use_poll_in_socket/?tl=fr

**Telnet**

- https://www.ibm.com/docs/fr/i/7.6.0?topic=services-telnet
