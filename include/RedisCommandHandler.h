#pragma once
#include <string>
#include <vector>
#include "RedisDatabase.h"

class RedisCommandHandler {
public:
    // Takes the parsed tokens and a reference to the database, returns the string response
    static std::string handle(const std::vector<std::string>& tokens, RedisDatabase& db);
};
