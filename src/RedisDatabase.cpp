#include "../include/RedisDatabase.h"
#include <iostream>

RedisDatabase::RedisDatabase() {
    load(); // Automatically load data when the server turns on
}

std::string RedisDatabase::set(const std::string& key, const std::string& value) {
    std::lock_guard<std::mutex> lock(db_mutex);
    store[key] = value;
    return "+OK\r\n";
}

std::string RedisDatabase::get(const std::string& key) {
    std::lock_guard<std::mutex> lock(db_mutex);
    if (store.find(key) != store.end()) {
        return "+" + store[key] + "\r\n";
    }
    return "$-1\r\n";
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
