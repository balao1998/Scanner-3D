// socket_client.h
#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <sys/select.h>

// --- CONFIGURAÇÃO DA COMUNICAÇÃO ---
#define SERVER_IP "10.225.134.252"
#define SERVER_PORT 23
#define BUFFER_SIZE 1024
#define END_MARKER 'e'

// --- VARIÁVEIS GLOBAIS ---
extern int sock_fd;
extern FILE *data_file;
extern char FILENAME[256];

// --- PROTÓTIPOS ---
int setup_socket_connection();
int send_command(const char *cmd);
void check_and_handle_reception();
void close_data_file();

#endif
