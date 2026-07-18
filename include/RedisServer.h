#pragma once
#include <iostream>

class RedisServer {
private:
    int port;
    int server_fd; 

public:
    RedisServer(int port = 6379);
    ~RedisServer();
    void run();
};
