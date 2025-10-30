// main.c
#include <stdio.h>
#include <unistd.h>   // para usleep()
#include "socket_client.h"

// Menu simples
char menu() {
    char c;
    printf("\nScanner 3D\n");
    printf("\ns-Inicio de leitura\n");
    printf("p-Parar leitura\n");
    printf("c-Calibrar\n");
    printf("h-Home\n");
    printf("q-Sair\n");
    printf("Status: %s\n", (data_file != NULL) ? FILENAME : "AGUARDANDO COMANDO");
    printf("Insira um comando: ");

    if (scanf(" %c", &c) != 1) {
        int tmp;
        while ((tmp = getchar()) != '\n' && tmp != EOF) {}
        return 'X';
    }

    int tmp;
    while ((tmp = getchar()) != '\n' && tmp != EOF) {}
    return c;
}

int main() {
    if (setup_socket_connection() != 0)
        return 1;

    while (1) {
        check_and_handle_reception();

        if (data_file != NULL) {
            usleep(50000);
            continue;
        }

        char comando = menu();

        switch (comando) {
            case 's': case 'S': {
                if (data_file == NULL) {
                    printf("Nome do ficheiro para registo (.txt): ");
                    if (scanf(" %255s", FILENAME) == 1) {
                        data_file = fopen(FILENAME, "a");
                        if (data_file != NULL) {
                            printf("\nFicheiro '%s' aberto. Enviando START...\n", FILENAME);
                            send_command("S");
                            int tmp;
                            while ((tmp = getchar()) != '\n' && tmp != EOF) {}
                            continue;
                        } else perror("ERRO: Falha ao abrir o ficheiro");
                    } else printf("Nome de ficheiro inválido.\n");
                    int tmp;
                    while ((tmp = getchar()) != '\n' && tmp != EOF) {}
                } else printf("AVISO: O registo já está ativo em '%s'.\n", FILENAME);
                break;
            }

            case 'p': case 'P':
                printf("Enviando PARAR...\n");
                send_command("P");
                check_and_handle_reception();
                close_data_file();
                break;

            case 'c': case 'C':
                printf("Enviando CALIBRAR...\n");
                send_command("C");
                break;

            case 'h': case 'H':
                printf("Enviando HOME...\n");
                send_command("H");
                break;

            case 'q': case 'Q':
                if (sock_fd >= 0) close(sock_fd);
                close_data_file();
                printf("Programa terminado.\n");
                return 0;

            default:
                if (comando != 'X')
                    printf("Comando inválido. Tente novamente.\n");
                break;
        }
    }
}
