# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Nome do executável
TARGET = programaTrab

# Arquivos fonte (todos os .c do diretório)
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

# Compilação (make all)
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Execução (make run)
run:
	./$(TARGET)

# Limpeza (opcional, não é executado automaticamente)
clean:
	rm -f $(OBJ) $(TARGET)
