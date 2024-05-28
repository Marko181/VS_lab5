# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Source files
SRC = cli_tcp.c cli_udp.c serv_tcp.c serv_udp.c

# Object files
OBJ = $(SRC:.c=.o)

# Executable names
EXE = cli_tcp cli_udp serv_tcp serv_udp

# Default target
all: $(EXE)

# Rule to create each executable
cli_tcp: cli_tcp.o
	$(CC) $(CFLAGS) -o $@ cli_tcp.o

cli_udp: cli_udp.o
	$(CC) $(CFLAGS) -o $@ cli_udp.o

serv_tcp: serv_tcp.o
	$(CC) $(CFLAGS) -o $@ serv_tcp.o

serv_udp: serv_udp.o
	$(CC) $(CFLAGS) -o $@ serv_udp.o

# Rule to create object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJ) $(EXE)

# Phony targets
.PHONY: all clean
