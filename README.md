# Build Your Own Redis - C++ In-Memory Database

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)
![License](https://img.shields.io/badge/license-MIT-blue.svg)

## Overview
A high-performance, multi-threaded, persistent in-memory Key-Value store built entirely from scratch in C++. This project is a custom implementation of a Redis-compatible server, engineered using raw POSIX sockets without any external dependencies. It demonstrates advanced systems programming concepts, including thread-safe concurrency, custom protocol parsing, and data persistence.

## Core Features
- **Custom TCP/IP Server:** Built from the ground up using POSIX sockets (`<sys/socket.h>`) for low-level network communication.
- **Concurrency & Thread Safety:** Employs a thread-per-client execution model using `std::thread`. All data structures are protected via `std::mutex` and `std::lock_guard`, ensuring safe concurrent reads and writes across multiple client connections.
- **Data Persistence:** Implements a snapshotting mechanism that saves the current state of memory to disk (`dump.txt`) and automatically reloads it upon server initialization.
- **Time-To-Live (TTL) / Expiration:** Features a lazy expiration system using `<chrono>` to track and automatically evict keys once their TTL has elapsed.
- **Custom RESP Parser:** Includes a custom parser for the standard Redis Serialization Protocol (RESP), allowing the server to understand and process standard client requests.

## Supported Commands

### Key/Value & Server
- `SET <key> <value>` - Set the string value of a key.
- `GET <key>` - Get the value of a key.
- `DEL <key>` - Delete a key.
- `EXISTS <key>` - Determine if a key exists.
- `KEYS` - Return all keys in the database.
- `SAVE` - Persist the database state to disk.
- `EXPIRE <key> <seconds>` - Set a key's time to live in seconds.
- `PING` - Test the server connection.

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

## Tech Stack
- **Language:** C++17
- **Networking:** POSIX Sockets API
- **Concurrency:** `std::thread`, `std::mutex`, `std::lock_guard`
- **Data Structures:** `std::unordered_map`, `std::vector`, `std::string`
- **Time/Metrics:** `std::chrono`

## How to Build and Run

Ensure you have a POSIX-compliant environment (Linux, macOS, or WSL on Windows) and `g++` installed.

1. Build the project using the provided Makefile:
```bash
make
```

2. Run the compiled server executable:
```bash
./server
```
*The server will start listening for incoming connections on port 6379.*

## How to Connect

You can connect to the server using standard tools like `nc` (netcat) or `redis-cli`. 

To connect using `nc`:
```bash
nc localhost 6379
```

**Example Session:**
```text
SET name John
+OK
GET name
+John
PING
+PONG
```