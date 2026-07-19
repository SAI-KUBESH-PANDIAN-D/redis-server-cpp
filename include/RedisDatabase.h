#pragma once
#include <string>
#include <unordered_map>
#include <mutex>
#include <fstream>
#include <vector>

class RedisDatabase {
private:
    std::unordered_map<std::string, std::string> store;
    std::unordered_map<std::string, std::vector<std::string>> list_store;
    // New store for Hashes (A map inside a map)
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> hash_store; 
    std::mutex db_mutex;

public:
    RedisDatabase();
    
    std::string set(const std::string& key, const std::string& value);
    std::string get(const std::string& key);
    std::string del(const std::string& key);
    std::string exists(const std::string& key);
    std::string keys();
    
    std::string lpush(const std::string& key, const std::string& value);
    std::string rpush(const std::string& key, const std::string& value);
    std::string lpop(const std::string& key);
    std::string rpop(const std::string& key);
    std::string llen(const std::string& key);

    // Hash Commands
    std::string hset(const std::string& key, const std::string& field, const std::string& value);
    std::string hget(const std::string& key, const std::string& field);
    std::string hdel(const std::string& key, const std::string& field);
    std::string hgetall(const std::string& key);

    void save();
    void load();
};
