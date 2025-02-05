/*
 * network.c - Source for my simple network implementation (Linux/Windows).
 *
 * Notes :
 *   - Converted from my network header library to header/source.
 *
 ****************************************************************************
 *
 * Author: Philip R. Simonson
 * Date  : 06/30/2022
 *
 ****************************************************************************
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "network.h"

/* Close a socket.
 */
void socket_close(SOCKET fd)
{
#if defined(_WIN32) || defined(_WIN64)
    closesocket(fd);
#else
    close(fd);
#endif
}

/* Open a server socket and bind.
 */
SOCKET socket_connect(char *server_name, unsigned short port)
{
	struct hostent *server;
	struct sockaddr_in addr;
	SOCKET fd;

	/* Get server info from host IP or name */
	server = gethostbyname(server_name);
	if(server == NULL) {
		/* Set errno */
#if defined(_WIN32) || defined(_WIN64)
        errno = ENETDOWN;
#else
		errno = EHOSTDOWN;
#endif
		return -1;
	}

	/* Open a socket */
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd == INVALID_SOCKET) {
		return -1;
	}

	/* Set up an address */
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	/* Copy the server address info */
	memcpy(&addr.sin_addr.s_addr, server->h_addr_list[0], server->h_length + 1);

	/* Connect to the server */
	if(connect(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in))) {
#if defined(_WIN32) || defined(_WIN64)
        closesocket(fd);
#else
		close(fd);
#endif
		return -1;
	}

	return fd;
}

/* Open a server that will accept TCP connections.
 */
SOCKET server_socket_open(unsigned short *port)
{
	struct sockaddr_in addr;
	socklen_t addrlen;
	SOCKET fd;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd == INVALID_SOCKET)
		return -1;

	/* Set up the server to listen */
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(*port);

	/* Bind the server socket to the address */
	if(bind(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in))) {
#if defined(_WIN32) || defined(_WIN64)
        closesocket(fd);
#else
		close(fd);
#endif
		return -1;
	}

	/* Listen for client connections. */
	if(listen(fd, TCPSOCKET_BACKLOG)) {
#if defined(_WIN32) || defined(_WIN64)
        closesocket(fd);
#else
		close(fd);
#endif
		return -1;
	}

	/* Get information about the new socket */
	addrlen = sizeof(struct sockaddr_in);
	if(getsockname(fd, (struct sockaddr *)&addr, &addrlen)) {
#if defined(_WIN32) || defined(_WIN64)
        closesocket(fd);
#else
		close(fd);
#endif
		return -1;
	}

	/* Read out the port information for the socket */
	*port = ntohs(addr.sin_port);

	/* Return the server socket */
	return fd;
}

/* Accept an incoming connection from a server socket.
 */
SOCKET server_socket_accept(SOCKET server_fd)
{
	struct sockaddr_in client_addr;
	socklen_t client_addrlen;
	SOCKET client_fd;

	/* Get size of sockaddr_in structure and store it */
	client_addrlen = sizeof(struct sockaddr_in);

	/* Block until we receive a connection or failure */
	client_fd = accept(server_fd,
					   (struct sockaddr *)&client_addr,
					   &client_addrlen);

	/* Handle error */
	if(client_fd == INVALID_SOCKET)
		return -1;

	return client_fd;
}

/* Get IP address from address structure.
 */
void *get_addr_in(struct sockaddr *sa)
{
	if(sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in *)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

/* Get peer address from socket.
 */
int get_addr(SOCKET fd, char *buf, int size)
{
	struct sockaddr_storage sa;
	socklen_t sa_len;

	if(size < INET6_ADDRSTRLEN)
		return 1;

	/* Get the address structure of the peer machine. */
	sa_len = sizeof(struct sockaddr_storage);
	if(getpeername(fd, (struct sockaddr *)&sa, &sa_len))
		return 2;

	inet_ntop(sa.ss_family,
			  get_addr_in((struct sockaddr *)&sa),
			  buf, size);

	return 0;
}
