// socket_client.c
#include "socket_client.h"

// --- VARIÁVEIS GLOBAIS ---
int sock_fd = -1;
FILE *data_file = NULL;
char FILENAME[256] = "dados.txt";

// =========================================================================
//                             FUNÇÕES DE SOCKET
// =========================================================================

int setup_socket_connection(const char *ip_address) {
    struct sockaddr_in server_addr;

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("Erro ao criar o socket");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, ip_address, &server_addr.sin_addr) <= 0) {
        perror("Endereço IP inválido/não suportado");
        close(sock_fd);
        return -1;
    }

    printf("A tentar conectar a %s:%d...\n", ip_address, SERVER_PORT);
    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Falha na conexão");
        close(sock_fd);
        return -1;
    }

    printf("Conexão estabelecida com sucesso! (%s)\n", ip_address);
    return 0;
}

int send_command(const char *cmd) {
    if (sock_fd < 0) {
        printf("ERRO: Socket não conectado.\n");
        return -1;
    }

    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%s\n", cmd);

    if (send(sock_fd, buffer, strlen(buffer), 0) < 0) {
        perror("Erro ao enviar dados");
        return -1;
    }
    return 0;
}

void close_data_file() {
    if (data_file != NULL) {
        fflush(data_file);
        fclose(data_file);
        data_file = NULL;
        printf("\n*** Registo parado e ficheiro '%s' fechado. ***\n", FILENAME);
    }
}

void check_and_handle_reception() {
    if (sock_fd < 0) return;

    fd_set read_fds;
    struct timeval tv;

    FD_ZERO(&read_fds);
    FD_SET(sock_fd, &read_fds);

    tv.tv_sec = 0;
    tv.tv_usec = 1000; // 1 ms (reduzido para ser mais rápido na resposta)

    if (select(sock_fd + 1, &read_fds, NULL, NULL, &tv) <= 0) {
        return; // Sem dados
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_received = recv(sock_fd, buffer, BUFFER_SIZE, 0);

    if (bytes_received > 0) {
        char *end_pos = (char *)memchr(buffer, END_MARKER, bytes_received);
       size_t data_size = (end_pos != NULL) ? (size_t)(end_pos - buffer) : (size_t)bytes_received;

        // --- ALTERAÇÃO 1: REMOVIDO O PRINT PARA O ECRÃ ---
        // printf("<- RECEBIDO: "); 
        // fwrite(buffer, 1, data_size, stdout);
        // fflush(stdout);
        // -------------------------------------------------

        if (data_file != NULL) {
            fwrite(buffer, 1, data_size, data_file);
            fflush(data_file);
            // Opcional: Imprimir um ponto para mostrar que está vivo sem sujar
            // printf("."); fflush(stdout); 
        }

        if (end_pos != NULL) {
            printf("\nATMega sinalizou FIM DE LEITURA ('%c').\n", END_MARKER);
            close_data_file();
        }
    } else if (bytes_received == 0) {
        printf("\nAtenção: O servidor (ESP01S) fechou a conexão.\n");
        close_data_file();
        close(sock_fd);
        sock_fd = -1;
    } else {
        perror("Erro de receção");
        close_data_file();
        close(sock_fd);
        sock_fd = -1;
    }
}