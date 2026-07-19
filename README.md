# Redis Server (C++ Implementation)

A custom, high-performance Redis Server built from scratch in C++. This project implements the core components of a Redis-compatible in-memory data structure store, including a custom TCP server, multi-client concurrency using threads, the Redis Serialization Protocol (RESP) parser, and persistent storage.

## Features

- **Multi-Client Concurrency:** Handles multiple simultaneous client connections using thread-per-client architecture (`std::thread`).
- **In-Memory Data Structures:** Supports Strings, Lists, and Hashes with thread-safe operations (`std::mutex`).
- **RESP Protocol Support:** Fully parses and responds using the standard Redis Serialization Protocol, meaning you can use the standard `redis-cli` to interact with it.
- **Data Persistence:** Can snapshot the current database state to disk (`dump.txt`) and automatically reload it upon startup.
- **Lazy Expiration:** Supports setting Time-To-Live (TTL) on keys, automatically expiring and removing them when accessed after the deadline.

## Supported Commands

### Server & Connection
- `PING` - Test the connection (returns `PONG`).
- `SAVE` - Persist the current database state to disk.

### String / Key-Value
- `SET <key> <value>` - Set the string value of a key.
- `GET <key>` - Get the value of a key.
- `DEL <key>` - Delete a key.
- `EXISTS <key>` - Determine if a key exists.
- `KEYS` - Return all keys in the database.
- `EXPIRE <key> <seconds>` - Set a key's time to live in seconds.

### Lists
- `LPUSH <key> <value>` - Prepend a value to a list.
- `RPUSH <key> <value>` - Append a value to a list.
- `LPOP <key>` - Remove and get the first element in a list.
- `RPOP <key>` - Remove and get the last element in a list.
- `LLEN <key>` - Get the length of a list.

### Hashes
- `HSET <key> <field> <value>` - Set the string value of a hash field.
- `HGET <key> <field>` - Get the value of a hash field.
- `HDEL <key> <field>` - Delete a hash field.
- `HGETALL <key>` - Get all the fields and values in a hash.

## Getting Started

### Prerequisites
- A POSIX-compliant environment (Linux, macOS, or WSL on Windows). Note: If building on Windows natively, use WSL or MinGW.
- `g++` compiler with C++11 (or higher) support.
- `pthread` library.

### Building the Project

To compile the server, run the following command from the root of the project directory:

```bash
g++ -o redis-server src/main.cpp src/RedisServer.cpp src/RedisDatabase.cpp -pthread
```

### Running the Server

Start the compiled server executable:

```bash
./redis-server
```

You should see output indicating the server has initialized and is listening on the default port `6379`:
```
Redis Server initialized!
Listening on port 6379...
```

### Connecting as a Client

You can connect to your custom server using the standard `redis-cli` tool from another terminal:

```bash
redis-cli -p 6379
127.0.0.1:6379> PING
PONG
127.0.0.1:6379> SET mykey "Hello World"
OK
127.0.0.1:6379> GET mykey
"Hello World"
```

## Architecture Summary

- **`RedisServer`**: Manages the TCP socket lifecycle, binds to port 6379, and listens for connections. It spawns a new worker thread for each client and handles tokenizing and routing RESP commands.
- **`RedisDatabase`**: The thread-safe storage engine. Uses nested `std::unordered_map` structures guarded by `std::mutex` for fast, concurrent data access across all data types (strings, lists, hashes). Also handles disk I/O for the `SAVE` command and startup recovery.