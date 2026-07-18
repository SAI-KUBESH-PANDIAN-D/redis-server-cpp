#pragma once
#include <string>
#include <unordered_map>
#include <mutex>
#include <fstream>

class RedisDatabase {
private:
    std::unordered_map<std::string, std::string> store;
    std::mutex db_mutex;

public:
    RedisDatabase(); // Constructor to load data on startup
    std::string set(const std::string& key, const std::string& value);
    std::string get(const std::string& key);
    
    void save(); // Saves data to disk
    void load(); // Loads data from disk
};
