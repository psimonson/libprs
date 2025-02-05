/*
 * network.h - Header for simple TCP/IP sockets.
 *
 * Author: Philip R. Simonson
 * Date  : 09/27/2021
 *
 *****************************************************************************
 */

#ifndef NETWORK_H
#define NETWORK_H

#ifndef TCPSOCKET_BACKLOG
#define TCPSOCKET_BACKLOG 32
#endif

#include <string.h>
#include <errno.h>

#if defined(_WIN32) || defined(_WIN64)
#ifndef _WIN32_LEAN_AND_MEAN
#define _WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#else
#define SOCKET int
#define INVALID_SOCKET -1

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

/* Close a socket. */
extern void socket_close(SOCKET fd);

/* Open a server socket and bind. */
extern SOCKET socket_connect(char *server_name, unsigned short port);

/* Open a server that will accept TCP connections. */
extern SOCKET server_socket_open(unsigned short *port);

/* Accept an incoming connection from a server socket. */
extern SOCKET server_socket_accept(SOCKET server_fd);

/* Get IP address from address structure. */
extern void *get_addr_in(struct sockaddr *sa);

/* Get peer address from socket. */
extern int get_addr(SOCKET fd, char *buf, int size);

#endif
