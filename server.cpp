#include "server.h"

int netw_server::start_server(int _port)
{
	port = _port;
	struct sockaddr_in address;
	int opt = 1;

	if ((serv_sock = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		return 1;

	if (setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		return 2;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	if (bind(serv_sock, reinterpret_cast<struct sockaddr*>(&address), sizeof(address)) < 0)
		return 3;

	if (listen(serv_sock, 5) < 0)
		return 4;

	int handler_return_val = server_handler(port);

	close(serv_sock);
	return handler_return_val;
}
int netw_server::client_handler(int client_sock)
{
	char buffer[4];
	printf("Send %d\n", port);
	int n_port = htonl(port);
	memcpy(buffer, &n_port, 4);
	if (sendall(client_sock, buffer, sizeof(buffer), 0))
		return 1;
	if (recvall(client_sock, buffer, sizeof(buffer), 0))
		return 1;
	memcpy(&n_port, buffer, 4);
	int h_port = ntohl(n_port);
	printf("Recv: %d\n", h_port);
	return 0;
}
int netw_server::server_handler(int port)
{
	int new_socket;
	struct sockaddr_in client_addr;
	socklen_t addrlen = sizeof(client_addr);
	if ((new_socket = accept(serv_sock, reinterpret_cast<struct sockaddr*>(&client_addr), &addrlen)) < 0)
		return 1;
	int handler_return_val = client_handler(new_socket);
	close(new_socket);
	return handler_return_val;
}

void netw_server_multiclient::broadcast_message(int sender, char* message, ssize_t size, int attempts, int flags)
{
	std::lock_guard<std::mutex> lock(clients_mutex);
	for (const client_t &client : clients)
		if (client.id != sender)
			if (sendall(client.id, message, size, flags))
				for (int i = 1; i < attempts; i++)
					if(!sendall(client.id, message, size, flags))
						break;
}

int netw_server_multiclient::client_handler(int client_sock)
{
	char buffer[4];
	printf("Send %d\n", port);
	int n_port = htonl(port);
	memcpy(buffer, &n_port, 4);
	if (sendall(client_sock, buffer, sizeof(buffer), 0))
		return 1;
	if (recvall(client_sock, buffer, sizeof(buffer), 0))
		return 1;
	memcpy(&n_port, buffer, 4);
	int h_port = ntohl(n_port);
	printf("Recv: %d\n", h_port);
	close(client_sock);
	return remove_client(client_sock);
}

int netw_server_multiclient::server_handler(int port)
{
	while (true)
	{
		int new_socket;
		struct sockaddr_in client_addr;
		socklen_t addrlen = sizeof(client_addr);
		if ((new_socket = accept(serv_sock, reinterpret_cast<struct sockaddr*>(&client_addr), &addrlen)) < 0)
		{
			continue;
		}
		//std::thread(this->client_handler, new_socket).detach();
		std::thread( [this, new_socket] { client_handler(new_socket); } ).detach();
	}
}

int netw_server_multiclient::remove_client(int sock)
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i].id == sock)
		{
			{
				std::vector<client_t>::iterator it = clients.begin();
				std::advance(it, i);
				clients.erase(it);
			}
			return 0;
		}
	}
	return 1;
}
