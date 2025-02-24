#pragma once
#include "netw_main.h"

struct netw_client
{
	int sock = -1;
	int port = -1;

	int start_client(const char* server_ip, int _port);

	virtual int client_handler(int sock);
};
