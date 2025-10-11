
CC = gcc
CFLAGS = -Wall -g -std=c11
LDFLAGS =

# Expand sources explicitly so make sees actual files (avoids literal "*.c" problems)
SRCS := $(wildcard *.c)

# Convert source filenames to object filenames
OBJS := $(patsubst %.c,%.o,$(SRCS))

TARGET = programaTrab

# binary artifacts (if any)
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

