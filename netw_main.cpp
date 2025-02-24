#include "netw_main.h"
#include <iostream>

// https://stackoverflow.com/a/27623860/16901918
int sendall(int sock, char* buff, ssize_t len, int flags)
{
	ssize_t total = 0;
	ssize_t bytes_left = len;

	while (total < len)
	{
		ssize_t bytes_sent;
		bytes_sent = send(sock, buff + total, bytes_left, flags);
		if (bytes_sent == -1)
			return 1;
		total += bytes_sent;
		bytes_left -= bytes_sent;
	}

	return 0;
}

// <sendall> modified for receiving
int recvall(int sock, char* buff, ssize_t len, int flags)
{
	ssize_t total = 0;
	ssize_t bytes_left = len;

	while (total < len)
	{
		ssize_t bytes_recv;
		bytes_recv = recv(sock, buff + total, bytes_left, flags);
		if (bytes_recv == -1)
			return 1;

		total += bytes_recv;
		bytes_left -= bytes_recv;
	}

	return 0;
}
