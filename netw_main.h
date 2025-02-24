#pragma once
#include <iostream>
#include <cstring>
#include <thread>
#include <vector>
#include <mutex>
#include <algorithm>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// https://stackoverflow.com/a/27623860/16901918
int sendall(int sock, char* buff, ssize_t len, int flags);

// <sendall> modified for receiving
int recvall(int sock, char* buff, ssize_t len, int flags);
int recvall(int sock, std::vector<uint8_t> buff, ssize_t len, int flags, int temp_size = 1024);
int recvall(int sock, std::string buff, ssize_t len, int flags, int temp_size = 1024);
