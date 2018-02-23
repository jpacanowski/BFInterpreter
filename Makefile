CC = gcc
CFLAGS = -O3 -W -Werror -Wall -Wextra -pedantic -std=c99
TARGET = brainfuck

all: $(TARGET)

$(TARGET): source/$(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) source/$(TARGET).c

clean:
	rm $(TARGET)