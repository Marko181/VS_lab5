# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Source files and corresponding executables
SRC = cli_tcp.c cli_udp.c serv_tcp.c serv_udp.c v_cli_tcp.c v_serv_tcp.c
EXE = cli_tcp cli_udp serv_tcp serv_udp v_cli_tcp v_serv_tcp

# Default target
all: $(EXE)

# Rule to create each executable directly from source files
cli_tcp: cli_tcp.c
	$(CC) $(CFLAGS) -o $@ cli_tcp.c

cli_udp: cli_udp.c
	$(CC) $(CFLAGS) -o $@ cli_udp.c

serv_tcp: serv_tcp.c
	$(CC) $(CFLAGS) -o $@ serv_tcp.c

serv_udp: serv_udp.c
	$(CC) $(CFLAGS) -o $@ serv_udp.c

cli_tcp: v_cli_tcp.c
	$(CC) $(CFLAGS) -o $@ v_cli_tcp.c

serv_tcp: v_serv_tcp.c
	$(CC) $(CFLAGS) -o $@ v_serv_tcp.c

# Clean up
clean:
	rm -f $(EXE)

# Phony targets
.PHONY: all clean
