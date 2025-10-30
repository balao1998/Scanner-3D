CC = gcc
TARGET = program
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS =

SRCS = main.c socket_client.c
OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	@echo "Ligando objetos para criar o executável '$(TARGET)'..."
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)
	@echo "Compilação concluída!"

%.o: %.c
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
	@echo "Limpeza concluída."

.PHONY: all clean
