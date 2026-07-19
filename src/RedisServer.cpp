    #include "../include/RedisServer.h"
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h>     
#include <cstring>
#include <thread>
#include <sstream>
#include <vector>

RedisServer::RedisServer(int port) {
    this->port = port;  
    this->server_fd = -1;
}

RedisServer::~RedisServer() {
    if (server_fd != -1) {
        close(server_fd);
    }
}

void RedisServer::handleClient(int client_fd) {
    char buffer[1024] = {0};
    while (true) {
        memset(buffer, 0, 1024);
        int bytes_read = read(client_fd, buffer, 1024);
        
        if (bytes_read <= 0) {
            std::cout << "Client disconnected.\n";
            break;
        }

        // Parse the input into tokens (words)
        std::string command(buffer);
        std::istringstream iss(command);
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }

        if (tokens.empty()) continue;

        std::string response;
        if (tokens[0] == "PING" || tokens[0] == "ping") {
            response = "+PONG\r\n";
        } 
        else if (tokens[0] == "SET" || tokens[0] == "set") {
            if (tokens.size() >= 3) response = db.set(tokens[1], tokens[2]);
            else response = "-ERR wrong number of arguments for 'set'\r\n";
        } 
        else if (tokens[0] == "GET" || tokens[0] == "get") {
            if (tokens.size() >= 2) response = db.get(tokens[1]);
            else response = "-ERR wrong number of arguments for 'get'\r\n";
        } 
        else if (tokens[0] == "SAVE" || tokens[0] == "save") {
            db.save();
            response = "+OK\r\n";
        }
        else if (tokens[0] == "DEL" || tokens[0] == "del") {
            if (tokens.size() >= 2) response = db.del(tokens[1]);
            else response = "-ERR wrong number of arguments for 'del'\r\n";
        }
        else if (tokens[0] == "EXISTS" || tokens[0] == "exists") {
            if (tokens.size() >= 2) response = db.exists(tokens[1]);
            else response = "-ERR wrong number of arguments for 'exists'\r\n";
        }
        else if (tokens[0] == "KEYS" || tokens[0] == "keys") {
            response = db.keys();
        }
        else if (tokens[0] == "LPUSH" || tokens[0] == "lpush") {
            if (tokens.size() >= 3) response = db.lpush(tokens[1], tokens[2]);
            else response = "-ERR wrong number of arguments for 'lpush'\r\n";
        }
        else if (tokens[0] == "RPUSH" || tokens[0] == "rpush") {
            if (tokens.size() >= 3) response = db.rpush(tokens[1], tokens[2]);
            else response = "-ERR wrong number of arguments for 'rpush'\r\n";
        }
        else if (tokens[0] == "LPOP" || tokens[0] == "lpop") {
            if (tokens.size() >= 2) response = db.lpop(tokens[1]);
            else response = "-ERR wrong number of arguments for 'lpop'\r\n";
        }
        else if (tokens[0] == "RPOP" || tokens[0] == "rpop") {
            if (tokens.size() >= 2) response = db.rpop(tokens[1]);
            else response = "-ERR wrong number of arguments for 'rpop'\r\n";
        }
        else if (tokens[0] == "LLEN" || tokens[0] == "llen") {
            if (tokens.size() >= 2) response = db.llen(tokens[1]);
            else response = "-ERR wrong number of arguments for 'llen'\r\n";
        }
        else if (tokens[0] == "HSET" || tokens[0] == "hset") {
            if (tokens.size() >= 4) response = db.hset(tokens[1], tokens[2], tokens[3]);
            else response = "-ERR wrong number of arguments for 'hset'\r\n";
        }
        else if (tokens[0] == "HGET" || tokens[0] == "hget") {
            if (tokens.size() >= 3) response = db.hget(tokens[1], tokens[2]);
            else response = "-ERR wrong number of arguments for 'hget'\r\n";
        }
        else if (tokens[0] == "HDEL" || tokens[0] == "hdel") {
            if (tokens.size() >= 3) response = db.hdel(tokens[1], tokens[2]);
            else response = "-ERR wrong number of arguments for 'hdel'\r\n";
        }
        else if (tokens[0] == "HGETALL" || tokens[0] == "hgetall") {
            if (tokens.size() >= 2) response = db.hgetall(tokens[1]);
            else response = "-ERR wrong number of arguments for 'hgetall'\r\n";
        }
        else {
            response = "-ERR unknown command\r\n";
        }

        write(client_fd, response.c_str(), response.length());
    }
    close(client_fd);
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
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            std::cerr << "Error: Failed to accept connection\n";
            continue;
        }

        std::cout << "A new client connected!\n";

        // Create a new thread (worker) for this client and store it in our vector
        client_threads.emplace_back(&RedisServer::handleClient, this, client_fd);
    }
}
