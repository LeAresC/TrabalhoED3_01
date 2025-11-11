CC = gcc
CFLAGS = -Iinclude -Wall -Wextra
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = programTrab

all: $(TARGET)

$(TARGET): $(OBJ)
	@$(CC) $(OBJ) -o $(TARGET)

run: $(TARGET)
	@./$(TARGET)

clean:
	@rm -f $(OBJ) $(TARGET)
