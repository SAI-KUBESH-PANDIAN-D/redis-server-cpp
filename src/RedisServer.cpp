#include "../include/RedisServer.h"
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h>     
#include <cstring>

RedisServer::RedisServer(int port) {
    this->port = port;
    this->server_fd = -1;
}

RedisServer::~RedisServer() {
    if (server_fd != -1) {
        close(server_fd);
    }
}

void RedisServer::run() {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Error: Cannot create socket\n";
        return;
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Error: Cannot set SO_REUSEADDR\n";
        return;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    server_addr.sin_port = htons(this->port); 

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Error: Bind failed.\n";
        return;
    }

    if (listen(server_fd, 10) < 0) {
        std::cerr << "Error: Listen failed\n";
        return;
    }

    std::cout << "Redis Server initialized!\n";
    std::cout << "Listening on port " << port << "...\n";

    while (true) {
        // Client acceptance logic goes here later
    }
}
