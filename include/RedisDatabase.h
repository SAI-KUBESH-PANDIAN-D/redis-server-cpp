#pragma once
#include <string>
#include <unordered_map>
#include <mutex>

class RedisDatabase {
private:
    std::unordered_map<std::string, std::string> store;
    std::mutex db_mutex;

public:
    std::string set(const std::string& key, const std::string& value);
    std::string get(const std::string& key);
};
