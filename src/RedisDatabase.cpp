#include "../include/RedisDatabase.h"
#include <iostream>

RedisDatabase::RedisDatabase() {
    load(); // Automatically load data when the server turns on
}

bool RedisDatabase::isExpired(const std::string& key) {
    if (expiry_store.find(key) != expiry_store.end()) {
        auto now = std::chrono::steady_clock::now();
        if (now > expiry_store[key]) {
            // Time is up! Delete from all stores
            store.erase(key);
            list_store.erase(key);
            hash_store.erase(key);
            expiry_store.erase(key);
            return true;
        }
    }
    return false;
}

std::string RedisDatabase::set(const std::string& key, const std::string& value) {
    std::lock_guard<std::mutex> lock(db_mutex);
    store[key] = value;
    return "+OK\r\n";
}

std::string RedisDatabase::get(const std::string& key) {
    std::lock_guard<std::mutex> lock(db_mutex);
    if (isExpired(key)) return "$-1\r\n"; // Check expiry first!
    
    if (store.find(key) != store.end()) {
        return "+" + store[key] + "\r\n";
    }
    return "$-1\r\n";
}

std::string RedisDatabase::expire(const std::string& key, int seconds) {
    std::lock_guard<std::mutex> lock(db_mutex);
    auto now = std::chrono::steady_clock::now();
    expiry_store[key] = now + std::chrono::seconds(seconds);
    return ":1\r\n";
}

void RedisDatabase::save() {
    std::lock_guard<std::mutex> lock(db_mutex);
    std::ofstream file("dump.txt");
    for (const auto& pair : store) {
        file << pair.first << " " << pair.second << "\n";
    }
    file.close();
    std::cout << "Database saved to disk.\n";
}

void RedisDatabase::load() {
    std::lock_guard<std::mutex> lock(db_mutex);
    std::ifstream file("dump.txt");
    if (!file.is_open()) return; // File doesn't exist yet, that's fine

    std::string key, value;
    while (file >> key >> value) {
        store[key] = value;
    }
    file.close();
    std::cout << "Database loaded from disk.\n";
}

std::string RedisDatabase::del(const std::string& key) {
    std::lock_guard<std::mutex> lock(db_mutex);
    if (store.erase(key)) {
        return ":1\r\n"; // Redis standard integer reply for 1 deleted
    }
    return ":0\r\n"; // 0 deleted
}

std::string RedisDatabase::exists(const std::string& key) {
    std::lock_guard<std::mutex> lock(db_mutex);
    if (store.find(key) != store.end()) {
        return ":1\r\n"; // Exists
    }
    return ":0\r\n"; // Does not exist
}

std::string RedisDatabase::keys() {
    std::lock_guard<std::mutex> lock(db_mutex);
    if (store.empty()) return "*0\r\n"; // Empty array

    std::string response = "*" + std::to_string(store.size()) + "\r\n";
    for (const auto& pair : store) {
        response += "$" + std::to_string(pair.first.length()) + "\r\n" + pair.first + "\r\n";
    }
    return response;
}

std::string RedisDatabase::lpush(const std::string& key, const std::string& value) {
    std::lock_guard<std::mutex> lock(db_mutex);
    list_store[key].insert(list_store[key].begin(), value);
    return ":" + std::to_string(list_store[key].size()) + "\r\n";
}

std::string RedisDatabase::rpush(const std::string& key, const std::string& value) {
    std::lock_guard<std::mutex> lock(db_mutex);
    list_store[key].push_back(value);
    return ":" + std::to_string(list_store[key].size()) + "\r\n";
}

std::string RedisDatabase::lpop(const std::string& key) {
    std::lock_guard<std::mutex> lock(db_mutex);
    if (list_store.find(key) == list_store.end() || list_store[key].empty()) {
        return "$-1\r\n"; // Null/Empty
    }
    std::string val = list_store[key].front();
    list_store[key].erase(list_store[key].begin());
    return "+" + val + "\r\n";
}

std::string RedisDatabase::rpop(const std::string& key) {
    std::lock_guard<std::mutex> lock(db_mutex);
    if (list_store.find(key) == list_store.end() || list_store[key].empty()) {
        return "$-1\r\n";
    }
    std::string val = list_store[key].back();
    list_store[key].pop_back();
    return "+" + val + "\r\n";
}

std::string RedisDatabase::llen(const std::string& key) {
    std::lock_guard<std::mutex> lock(db_mutex);
    if (list_store.find(key) == list_store.end()) {
        return ":0\r\n";
    }
    return ":" + std::to_string(list_store[key].size()) + "\r\n";
}

std::string RedisDatabase::hset(const std::string& key, const std::string& field, const std::string& value) {
    std::lock_guard<std::mutex> lock(db_mutex);
    hash_store[key][field] = value;
    return ":1\r\n"; // 1 means field was added
}

std::string RedisDatabase::hget(const std::string& key, const std::string& field) {
    std::lock_guard<std::mutex> lock(db_mutex);
    if (hash_store.find(key) != hash_store.end() && hash_store[key].find(field) != hash_store[key].end()) {
        return "+" + hash_store[key][field] + "\r\n";
    }
    return "$-1\r\n"; // Null
}

std::string RedisDatabase::hdel(const std::string& key, const std::string& field) {
    std::lock_guard<std::mutex> lock(db_mutex);
    if (hash_store.find(key) != hash_store.end()) {
        if (hash_store[key].erase(field)) {
            return ":1\r\n"; // 1 means deleted
        }
    }
    return ":0\r\n"; // 0 means not found
}

std::string RedisDatabase::hgetall(const std::string& key) {
    std::lock_guard<std::mutex> lock(db_mutex);
    if (hash_store.find(key) == hash_store.end() || hash_store[key].empty()) {
        return "*0\r\n"; // Empty array
    }
    
    auto& inner_map = hash_store[key];
    std::string response = "*" + std::to_string(inner_map.size() * 2) + "\r\n"; // Size * 2 because we return field AND value
    
    for (const auto& pair : inner_map) {
        response += "$" + std::to_string(pair.first.length()) + "\r\n" + pair.first + "\r\n";
        response += "$" + std::to_string(pair.second.length()) + "\r\n" + pair.second + "\r\n";
    }
    return response;
}
