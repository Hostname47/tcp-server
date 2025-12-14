CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LIBS = 

all: server client

server: server.c
	$(CC) $(CFLAGS) -o server server.c $(LIBS)
	@echo "✓ Server compiled"

client: client.c
	$(CC) $(CFLAGS) -o client client.c $(LIBS)
	@echo "✓ Client compiled"

# Run server in background (recommended: use separate terminal)
run-server: server
	@echo "Starting TCP/IP Demonstration Server on port 9999..."
	@echo "Keep this running and open another terminal for the client"
	./server

# Run client with default message
run-client: client
	./client

# Run client with custom message
msg: client
	./client "$(MSG)"

# Example demonstrations
demo1: client
	@echo "\n=== Demo 1: Simple Hello Message ==="
	./client "HELLO_WORLD"

demo2: client
	@echo "\n=== Demo 2: Longer Message ==="
	./client "This is a longer message to demonstrate TCP transmission"

demo3: client
	@echo "\n=== Demo 3: Special Characters ==="
	./client "Message with numbers: 12345"

# Clean
clean:
	rm -f server client
	@echo "✓ Cleaned up binaries"

.PHONY: all run-server run-client msg demo1 demo2 demo3 clean