CC = gcc
CFLAGS = -Wall -Iinclude -g -shared -fPIC

TARGET = monitor.so

SRCS = src/main.c src/hashmap.c src/parser.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)