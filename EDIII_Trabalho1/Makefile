CC = gcc
CFLAGS = -Wall -g -std=c11
LDFLAGS = -lm

SRCS = *.c

OBJS = $(SRCS:.c=.o)

TARGET = programaTrab

BINS = *.bin

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET) $(BINS)

