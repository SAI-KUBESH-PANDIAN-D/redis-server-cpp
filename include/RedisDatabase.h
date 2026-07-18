#pragma once
#include <string>
#include <unordered_map>
#include <mutex>
#include <fstream>
#include <vector>

class RedisDatabase {
private:
    std::unordered_map<std::string, std::string> store;
    std::mutex db_mutex;

public:
    RedisDatabase();
    std::string set(const std::string& key, const std::string& value);
    std::string get(const std::string& key);
    
    // New Commands
    std::string del(const std::string& key);
    std::string exists(const std::string& key);
    std::string keys();
    
    void save();
    void load();
};
