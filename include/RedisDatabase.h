#pragma once
#include <string>
#include <unordered_map>
#include <mutex>
#include <fstream>
#include <vector>

class RedisDatabase {
private:
    std::unordered_map<std::string, std::string> store;
    std::unordered_map<std::string, std::vector<std::string>> list_store; // New store for lists
    std::mutex db_mutex;

public:
    RedisDatabase();
    // String KV Commands
    std::string set(const std::string& key, const std::string& value);
    std::string get(const std::string& key);
    std::string del(const std::string& key);
    std::string exists(const std::string& key);
    std::string keys();
    
    // List Commands
    std::string lpush(const std::string& key, const std::string& value);
    std::string rpush(const std::string& key, const std::string& value);
    std::string lpop(const std::string& key);
    std::string rpop(const std::string& key);
    std::string llen(const std::string& key);

    void save();
    void load();
};
