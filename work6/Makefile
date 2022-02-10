CC := gcc
CFLAGS := -Wall -Werror -c -g
TARGET := bin/calc

all: bin $(TARGET)

$(TARGET): src/main.c src/foo.c
	$(CC) $(CFLAGS) $< -o $@

bin:
	mkdir bin

clean:
	rm -rf $(TARGET)