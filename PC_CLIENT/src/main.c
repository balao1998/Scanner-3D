#include "socket_client.h"
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

void clear_screen() { printf("\033[H\033[J"); }

void flush_input() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) {
  }
}

void capture_loop() {
  clear_screen();
  printf("\n==========================================\n");
  printf("   A GRAVAR DADOS NO FICHEIRO: %s\n", FILENAME);
  printf("==========================================\n");
  printf("-> A receber dados do scanner...\n");
  printf("-> Pressione [p] e [ENTER] para parar manualmente.\n");

  while (data_file != NULL) {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(sock_fd, &fds);      // Monitor socket
    FD_SET(STDIN_FILENO, &fds); // Monitor Keyboard

    struct timeval tv = {0, 100000}; // Timeout 100ms
    int max_fd = (sock_fd > STDIN_FILENO) ? sock_fd : STDIN_FILENO;

    select(max_fd + 1, &fds, NULL, NULL, &tv);

    if (FD_ISSET(sock_fd, &fds)) {
      check_and_handle_reception();
    }

    if (data_file != NULL && FD_ISSET(STDIN_FILENO, &fds)) {
      char c;
      if (read(STDIN_FILENO, &c, 1) > 0) {
        if (c == 'p' || c == 'P') {
          printf("\n>> Comando de paragem recebido.\n");
          send_command("P");
          close_data_file();
        }
      }
    }
  }

  printf("\n--- Fim da captura. A regressar ao menu... ---\n");
  sleep(1); // Brief pause to see the log
}

// ==========================================
// Main Menu
// ==========================================

char menu() {
  char c;
  printf("\n=== SCANNER 3D - CONTROLO REMOTO ===\n");
  printf(" [s] - Iniciar leitura (Start)\n");
  printf(" [p] - Parar leitura (Stop manual)\n");
  printf(" [c] - Calibrar Scanner\n");
  printf(" [h] - Ir para Home\n");
  printf(" [q] - Sair\n");
  printf("------------------------------------\n");
  printf("Status: %s\n", (data_file != NULL) ? "GRAVANDO..." : "PRONTO");
  printf("Comando > ");

  if (scanf(" %c", &c) != 1)
    return 'X';

  flush_input(); // Clear input buffer
  return c;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    clear_screen();
    printf("ERRO: IP em falta.\nUso: %s <IP_DO_SERVIDOR>\n", argv[0]);
    return 1;
  }

  const char *ip = argv[1];
  clear_screen();
  printf("A ligar a %s ...\n", ip);

  if (setup_socket_connection(ip) != 0)
    return 1;

  while (1) {
    // file opened enter capture loop
    if (data_file != NULL) {
      capture_loop();
      continue;
    }

    check_and_handle_reception();

    clear_screen();
    char comando = menu();

    printf("\n");

    switch (comando) {
    case 's':
    case 'S':
      if (data_file == NULL) {
        printf("Nome do ficheiro (.xyz): ");
        if (scanf(" %255s", FILENAME) == 1) {
          flush_input();

          data_file = fopen(FILENAME, "a");
          if (data_file != NULL) {
            printf(">> Ficheiro aberto. Enviando START...\n");
            send_command("S");
          } else {
            perror(">> ERRO ao criar ficheiro");
            sleep(2);
          }
        }
      } else {
        printf(">> AVISO: Já está a gravar!\n");
        sleep(1);
      }
      break;

    case 'p':
    case 'P':
      printf(">> Enviando PARAR...\n");
      send_command("P");
      close_data_file();
      sleep(1);
      break;

    case 'c':
    case 'C':
      printf(">> Enviando CALIBRAR...\n");
      send_command("C");
      sleep(1);
      break;

    case 'h':
    case 'H':
      printf(">> Enviando HOME...\n");
      send_command("H");
      sleep(1);
      break;

    case 'q':
    case 'Q':
      printf(">> A desligar...\n");
      if (sock_fd >= 0)
        close(sock_fd);
      close_data_file();
      return 0;

    default:
      printf(">> Comando inválido.\n");
      sleep(1);
      break;
    }
  }
}
