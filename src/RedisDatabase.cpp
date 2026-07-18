#include "../include/RedisDatabase.h"

std::string RedisDatabase::set(const std::string& key, const std::string& value) {
    std::lock_guard<std::mutex> lock(db_mutex); // Lock so threads don't collide
    store[key] = value;
    return "+OK\r\n";
}

std::string RedisDatabase::get(const std::string& key) {
    std::lock_guard<std::mutex> lock(db_mutex);
    if (store.find(key) != store.end()) {
        return "+" + store[key] + "\r\n"; // Found it
    }
    return "$-1\r\n"; // Redis standard for "Not Found (Null)"
}
