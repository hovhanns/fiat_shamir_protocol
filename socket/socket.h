#ifndef SOCKET_H
#define SOCKET_H 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>

#define MSG_SIZE 1408
#define Log printf

int socket_init(char *hostname, char *port);
char *get_data_from_socket(int sock);
int Listener(char *port);

#endif