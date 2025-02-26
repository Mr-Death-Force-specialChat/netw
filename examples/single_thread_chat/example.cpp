#include <iostream>
#include "../../server.h"
#include "../../client.h"

struct netw_server_messages : netw_server
{
	int flag_exit = 0;
	void receive_handler(int client_sock)
	{
		std::string message;
		while (!flag_exit && message != "QUIT")
		{
			uint16_t size;
			recv(client_sock, &size, 2, 0);
			size = ntohs(size);
			if (size > 512)
			{
				send(client_sock, "QUIT", 4, 0);
				flag_exit = 1;
				break;
			}
			char* buffer = reinterpret_cast<char*>(malloc(size));
			recvall(client_sock, buffer, size, 0);
			message = std::string(buffer, size);
			free(buffer);
			printf("\t\t\t\t\t\t\t\tClient: %s\nServer: ", message.c_str());
			fflush(stdout);
		}
		if (message == "QUIT")
		{
			uint16_t size = htons(4);
			send(client_sock, &size, 2, 0);
			send(client_sock, "QUIT", 4, 0);
			send(client_sock, &size, 2, 0);
			send(client_sock, "QUIT", 4, 0);
		}
	}

	int client_handler(int client_sock) override
	{
		std::thread( [this, client_sock] { receive_handler(client_sock); } ).detach();
		std::string message;
		while (!flag_exit && message != "QUIT")
		{
			printf("Server: ");
			fflush(stdout);
			getline(std::cin, message);
			uint16_t size = message.length();
			uint16_t sizen = htons(size);
			send(client_sock, &sizen, 2, 0);
			char* buffer = reinterpret_cast<char*>(malloc(size));
			message.copy(buffer, size);
			sendall(client_sock, buffer, size, 0);
			free(buffer);
		}
		return 0;
	}
};

struct netw_client_messages : netw_client
{
	int flag_exit = 0;
	void receive_handler(int sock)
	{
		std::string message;
		while (!flag_exit && message != "QUIT")
		{
			uint16_t size;
			recv(sock, &size, 2, 0);
			size = ntohs(size);
			if (size > 512)
			{
				send(sock, "QUIT", 4, 0);
				flag_exit = 1;
				break;
			}
			char* buffer = reinterpret_cast<char*>(malloc(size));
			recvall(sock, buffer, size, 0);
			message = std::string(buffer, size);
			free(buffer);
			printf("\t\t\t\t\t\t\t\tServer: %s\nClient: ", message.c_str());
			fflush(stdout);
		}
		if (message == "QUIT")
		{
			uint16_t size = htons(4);
			send(sock, &size, 2, 0);
			send(sock, "QUIT", 4, 0);
			send(sock, &size, 2, 0);
			send(sock, "QUIT", 4, 0);
		}
	}

	int client_handler(int sock) override
	{
		std::thread( [this, sock] { receive_handler(sock); } ).detach();
		std::string message;
		while (!flag_exit && message != "QUIT")
		{
			printf("Client: ");
			fflush(stdout);
			getline(std::cin, message);
			uint16_t size = message.length();
			uint16_t sizen = htons(size);
			send(sock, &sizen, 2, 0);
			char* buffer = reinterpret_cast<char*>(malloc(size));
			message.copy(buffer, size);
			sendall(sock, buffer, size, 0);
			free(buffer);
		}
		return 0;
	}
};

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Usage: [opt] [ip]\n"
				"s: server\n"
				"c: client\n");
		return -1;
	}
	if (argv[1][0] == 's')
	{
		printf("Running server at port [4231]\n");
		netw_server_messages server;
		server.start_server(4231);
		return 0;
	}
	else if (argv[1][0] == 'c')
	{
		const char* ip = "127.0.0.1";
		if (argc < 3)
			printf("Using loopback ip address\n");
		else
			ip = argv[2];
		printf("Running client at port [4231] [%s]\n", ip);
		netw_client_messages client;
		client.start_client(ip, 4231);
		return 0;
	}
	else
	{
		printf("Unknown flag %c [%02X]\n"
				"Run without arguments to print help.\n", argv[1][0], argv[1][0]);
		return -1;
	}
}
