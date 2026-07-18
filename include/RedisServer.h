#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include "RedisDatabase.h"

class RedisServer {
private:
    int port;
    int server_fd;
    std::vector<std::thread> client_threads; // Stores our workers
    RedisDatabase db;

    // The function that each worker thread will run
    void handleClient(int client_fd);

public:
    RedisServer(int port = 6379);
    ~RedisServer();
    void run();
};
