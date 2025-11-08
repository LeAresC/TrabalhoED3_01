
CC = gcc
CFLAGS = -Wall -g -std=c11 -Wextra
LDFLAGS =

# Arquivos fonte (todos os .c no diretório)
SRCS := $(wildcard *.c)

# Objetos gerados a partir das fontes
OBJS := $(patsubst %.c,%.o,$(SRCS))

# Nome do executável final
TARGET = programaTrab

# Arquivos binários gerados pelos testes (limpeza)
BINS = *.bin

# Regra padrão: gerar o alvo
all: $(TARGET)

# Linka todos os objetos para criar o executável
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compila cada .c em .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Executa o programa (leva em conta que 'all' já constrói)
run: all
	./$(TARGET)

# Limpa artefatos de build
clean:
	rm -f $(OBJS) $(TARGET) $(BINS)