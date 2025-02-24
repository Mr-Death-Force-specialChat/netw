#pragma once
#include "netw_main.h"

struct client_t
{
	int id;
	std::string username;
};

struct netw_server
{
	int serv_sock = -1;
	int port = -1;

	virtual int start_server(int _port);

	virtual int client_handler(int client_sock);

	virtual int server_handler(int port);
};

struct netw_server_multiclient : netw_server
{
	std::vector<client_t> clients;
	std::mutex clients_mutex;

	void broadcast_message(int sender, char* message, ssize_t size, int attempts = 3, int flags = 0);

	int client_handler(int client_sock) override;

	int server_handler(int port) override;

	int remove_client(int sock);
};
