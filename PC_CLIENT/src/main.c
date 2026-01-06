#include "socket_client.h"
#include <stdio.h>
#include <string.h>
#include <sys/select.h> // Para monitorizar teclado e socket
#include <unistd.h>     // Para sleep() e read()

// ==========================================
// FUNÇÕES AUXILIARES DE INTERFACE
// ==========================================

// Limpa o terminal usando códigos ANSI (rápido e sem flicker)
void clear_screen() { printf("\033[H\033[J"); }

// Limpa o buffer do teclado (consome 'enters' que sobram)
void flush_input() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) {
  }
}

// ==========================================
// LÓGICA DE CAPTURA DE DADOS
// ==========================================

void loop_de_captura() {
  clear_screen(); // Limpa tudo ao entrar no modo de gravação
  printf("\n==========================================\n");
  printf("   A GRAVAR DADOS NO FICHEIRO: %s\n", FILENAME);
  printf("==========================================\n");
  printf("-> A receber dados do scanner...\n");
  printf("-> Pressione [p] e [ENTER] para parar manualmente.\n");

  while (data_file != NULL) {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(sock_fd, &fds);      // Monitorizar Socket
    FD_SET(STDIN_FILENO, &fds); // Monitorizar Teclado

    struct timeval tv = {0, 100000}; // Timeout 100ms
    int max_fd = (sock_fd > STDIN_FILENO) ? sock_fd : STDIN_FILENO;

    // Espera atividade
    select(max_fd + 1, &fds, NULL, NULL, &tv);

    // 1. DADOS NO SOCKET (PRIORIDADE)
    if (FD_ISSET(sock_fd, &fds)) {
      check_and_handle_reception(); // Lê e grava no ficheiro
    }

    // 2. INPUT DO UTILIZADOR (PARAR)
    if (data_file != NULL && FD_ISSET(STDIN_FILENO, &fds)) {
      char c;
      if (read(STDIN_FILENO, &c, 1) > 0) {
        if (c == 'p' || c == 'P') {
          printf("\n>> Comando de paragem recebido.\n");
          send_command("P");
          close_data_file();

          // Limpar resto da linha se houver
          char trash;
          // flag O_NONBLOCK seria ideal, mas aqui um loop simples ajuda
          // Se o user carregou 'p' + 'enter', o enter será consumido no menu a
          // seguir
        }
      }
    }
  }

  printf("\n--- Fim da captura. A regressar ao menu... ---\n");
  sleep(1); // Pausa breve para ver que acabou
}

// ==========================================
// MENU PRINCIPAL
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

  // Leitura segura
  if (scanf(" %c", &c) != 1)
    return 'X';

  flush_input(); // Limpa o buffer
  return c;
}

// ==========================================
// MAIN
// ==========================================

int main(int argc, char *argv[]) {
  if (argc < 2) {
    clear_screen();
    printf("ERRO: IP em falta.\nUso: %s <IP_DO_SERVIDOR>\n", argv[0]);
    return 1;
  }

  char *ip = argv[1];
  clear_screen();
  printf("A ligar a %s ...\n", ip);

  if (setup_socket_connection(ip) != 0)
    return 1;

  // LOOP PRINCIPAL DO PROGRAMA
  while (1) {
    // Se o ficheiro estiver aberto, entra direto no modo de captura
    if (data_file != NULL) {
      loop_de_captura();
      continue;
    }

    // Processa mensagens de fundo (ex: ack do servidor)
    check_and_handle_reception();

    // 1. Limpa e mostra o Menu
    clear_screen();
    char comando = menu();

    printf("\n"); // Espaço visual

    // 2. Executa comando
    switch (comando) {
    case 's':
    case 'S':
      if (data_file == NULL) {
        printf("Nome do ficheiro (.xyz): ");
        if (scanf(" %255s", FILENAME) == 1) {
          flush_input(); // Limpa enter

          data_file = fopen(FILENAME, "a");
          if (data_file != NULL) {
            printf(">> Ficheiro aberto. Enviando START...\n");
            send_command("S");
            // O loop reinicia e entra no 'if (data_file != NULL)'
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
      // Stop manual fora do loop de captura (segurança)
      printf(">> Enviando PARAR...\n");
      send_command("P");
      close_data_file();
      sleep(1);
      break;

    case 'c':
    case 'C':
      printf(">> Enviando CALIBRAR...\n");
      send_command("C");
      sleep(1); // Pausa para ler feedback
      break;

    case 'h':
    case 'H':
      printf(">> Enviando HOME...\n");
      send_command("H");
      sleep(1); // Pausa para ler feedback
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
      sleep(1); // Pausa para ver o erro
      break;
    }
  }
}
