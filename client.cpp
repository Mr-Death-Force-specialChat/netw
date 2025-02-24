#include "client.h"

int netw_client::start_client(const char* server_ip, int _port)
{
	port = _port;
	sock = 0;
	struct sockaddr_in serv_addr;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return 1;

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0)
		return 2;

	if (connect(sock, reinterpret_cast<struct sockaddr*>(&serv_addr), sizeof(serv_addr)) < 0)
		return 3;

	int handler_return_val = client_handler(sock);

	close(sock);
	return handler_return_val;
}

int netw_client::client_handler(int sock)
{
	char buffer[4];
	int h_port;
	if (recvall(sock, buffer, sizeof(buffer), 0))
		return 1;
	memcpy(&h_port, buffer, 4);
	h_port = ntohl(h_port);
	printf("Recv %d\n", h_port);
	int n_port = htonl(h_port + 1);
	memcpy(buffer, &n_port, 4);
	if (sendall(sock, buffer, sizeof(buffer), 0))
		return 1;
	printf("Send: %d\n", h_port + 1);
	return 0;
}
