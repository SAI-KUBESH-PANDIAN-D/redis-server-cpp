#include "../include/RedisCommandHandler.h"

std::string RedisCommandHandler::handle(const std::vector<std::string>& tokens, RedisDatabase& db) {
    if (tokens.empty()) return "";

    std::string cmd = tokens[0];
    // Convert command to uppercase for easy matching
    for (auto & c: cmd) c = toupper(c);

    if (cmd == "PING") {
        return "+PONG\r\n";
    } 
    else if (cmd == "SET" && tokens.size() >= 3) return db.set(tokens[1], tokens[2]);
    else if (cmd == "GET" && tokens.size() >= 2) return db.get(tokens[1]);
    else if (cmd == "DEL" && tokens.size() >= 2) return db.del(tokens[1]);
    else if (cmd == "EXISTS" && tokens.size() >= 2) return db.exists(tokens[1]);
    else if (cmd == "KEYS") return db.keys();
    else if (cmd == "SAVE") {
        db.save();
        return "+OK\r\n";
    }
    else if (cmd == "LPUSH" && tokens.size() >= 3) return db.lpush(tokens[1], tokens[2]);
    else if (cmd == "RPUSH" && tokens.size() >= 3) return db.rpush(tokens[1], tokens[2]);
    else if (cmd == "LPOP" && tokens.size() >= 2) return db.lpop(tokens[1]);
    else if (cmd == "RPOP" && tokens.size() >= 2) return db.rpop(tokens[1]);
    else if (cmd == "LLEN" && tokens.size() >= 2) return db.llen(tokens[1]);
    else if (cmd == "HSET" && tokens.size() >= 4) return db.hset(tokens[1], tokens[2], tokens[3]);
    else if (cmd == "HGET" && tokens.size() >= 3) return db.hget(tokens[1], tokens[2]);
    else if (cmd == "HDEL" && tokens.size() >= 3) return db.hdel(tokens[1], tokens[2]);
    else if (cmd == "HGETALL" && tokens.size() >= 2) return db.hgetall(tokens[1]);
    else if (cmd == "EXPIRE" && tokens.size() >= 3) return db.expire(tokens[1], std::stoi(tokens[2]));
    
    return "-ERR unknown command or wrong number of arguments\r\n";
}
