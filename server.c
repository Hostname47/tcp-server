/* 
 * TCP/IP Protocol Demonstration - SERVER
 * This demonstrates TCP/IP protocol layers in action
 * 
 * TCP/IP Model Layers:
 * - Application Layer: Message handling (what we want to send)
 * - Transport Layer: TCP protocol (socket operations)
 * - Internet Layer: IP routing (handled by kernel)
 * - Network Access Layer: MAC addressing & physical transmission (hardware)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

#define PORT 9999
#define BUFFER_SIZE 256

/* Application Layer: Define message structure */
typedef struct {
    char timestamp[30];
    char client_ip[INET_ADDRSTRLEN];
    char message[200];
    int message_length;
} ApplicationMessage;

/* Helper to get current timestamp */
void get_timestamp(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", timeinfo);
}

/* Application Layer: Process received message */
void process_message(ApplicationMessage *msg) {
    printf("\n=== APPLICATION LAYER ===\n");
    printf("Received Message Structure:\n");
    printf("  Timestamp: %s\n", msg->timestamp);
    printf("  Client IP: %s\n", msg->client_ip);
    printf("  Message: %s\n", msg->message);
    printf("  Length: %d bytes\n", msg->message_length);
}

/* Transport Layer: Send data via TCP */
void send_via_tcp(int socket_fd, const char *data, size_t length) {
    printf("\n=== TRANSPORT LAYER (TCP) ===\n");
    printf("TCP Segment to send:\n");
    printf("  Source Port: 9999\n");
    printf("  Destination Port: (client ephemeral)\n");
    printf("  Data Size: %zu bytes\n", length);
    printf("  Flags: ACK, PSH (data push)\n");
    
    ssize_t sent = send(socket_fd, data, length, 0);
    printf("  Sent: %zd bytes\n", sent);
    printf("  TCP will handle: sequencing, checksums, retransmission\n");
}

/* Transport Layer: Receive data via TCP */
void receive_via_tcp(int socket_fd, char *buffer, size_t max_length) {
    printf("\n=== TRANSPORT LAYER (TCP) ===\n");
    printf("Waiting for TCP segment...\n");
    printf("  Listening on Port: 9999\n");
    printf("  Socket FD: %d\n", socket_fd);
    
    ssize_t received = recv(socket_fd, buffer, max_length - 1, 0);
    if (received > 0) {
        buffer[received] = '\0';
        printf("  Received: %zd bytes\n", received);
        printf("  TCP handled: sequencing, checksums, flow control\n");
    }
}

/* Network Layer: Show IP information */
void show_network_layer(const char *client_ip, uint16_t port) {
    printf("\n=== NETWORK LAYER (IP) ===\n");
    printf("IP Packet Information:\n");
    printf("  Source IP: 127.0.0.1 (local)\n");
    printf("  Destination IP: %s (client)\n", client_ip);
    printf("  Protocol: TCP (6)\n");
    printf("  TTL: 64 (hops)\n");
    printf("  Kernel handles: routing, fragmentation, reassembly\n");
}

/* Data Link Layer: Show MAC information (simulated) */
void show_datalink_layer() {
    printf("\n=== DATA LINK LAYER (Ethernet) ===\n");
    printf("Frame Information:\n");
    printf("  Source MAC: (system MAC address)\n");
    printf("  Destination MAC: (resolved via ARP)\n");
    printf("  Frame Type: 0x0800 (IPv4)\n");
    printf("  Hardware handles: MAC addressing, frame formatting\n");
}

int main() {
    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║      TCP/IP Protocol Demonstration - SERVER            ║\n");
    printf("║      Research Project on TCP/IP Layers                 ║\n");
    printf("╚════════════════════════════════════════════════════════╝\n");

    /* TRANSPORT LAYER: Create TCP socket */
    printf("\n>>> Creating TCP Socket (Transport Layer)\n");
    int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    printf("    Socket created: FD=%d, Protocol=TCP\n", server_socket);

    /* TRANSPORT LAYER: Socket options */
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    /* NETWORK LAYER: Bind to IP and port */
    printf("\n>>> Binding to IP:Port (Network Layer)\n");
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;              /* IPv4 */
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* All interfaces */
    server_addr.sin_port = htons(PORT);            /* Port 9999 */
    
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }
    printf("    Bound to: 0.0.0.0:%d\n", PORT);
    printf("    htons() converts port to Network Byte Order (Big Endian)\n");

    /* TRANSPORT LAYER: Listen for connections */
    printf("\n>>> Listening for Connections (Transport Layer)\n");
    if (listen(server_socket, 5) < 0) {
        perror("Listen failed");
        exit(1);
    }
    printf("    Server listening... (TCP backlog: 5)\n");
    printf("    Waiting for TCP 3-way handshake from client\n");

    while (1) {
        /* TRANSPORT LAYER: Accept connection (3-way handshake) */
        printf("\n╔════════════════════════════════════════════════════════╗\n");
        printf("║ Accepting New Connection (TCP 3-way Handshake)        ║\n");
        printf("╚════════════════════════════════════════════════════════╝\n");
        
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        /* NETWORK LAYER: Extract client IP */
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        uint16_t client_port = ntohs(client_addr.sin_port);

        printf("\n>>> TCP 3-Way Handshake Complete!\n");
        printf("    Client IP: %s\n", client_ip);
        printf("    Client Port: %d (ephemeral)\n", client_port);
        printf("    Server Port: %d\n", PORT);
        printf("    Connection State: ESTABLISHED\n");

        /* Show network and data link layers */
        show_network_layer(client_ip, client_port);
        show_datalink_layer();

        /* TRANSPORT LAYER: Receive data */
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);
        receive_via_tcp(client_socket, buffer, BUFFER_SIZE);

        /* APPLICATION LAYER: Process message */
        ApplicationMessage msg;
        get_timestamp(msg.timestamp, sizeof(msg.timestamp));
        strncpy(msg.client_ip, client_ip, INET_ADDRSTRLEN - 1);
        strncpy(msg.message, buffer, sizeof(msg.message) - 1);
        msg.message_length = strlen(buffer);
        
        process_message(&msg);

        /* APPLICATION LAYER: Prepare response */
        printf("\n=== APPLICATION LAYER (Server Response) ===\n");
        char response[BUFFER_SIZE];
        snprintf(response, BUFFER_SIZE, "SERVER ACK: Received '%s' (%d bytes)", 
                 msg.message, msg.message_length);
        printf("Response message: %s\n", response);

        /* TRANSPORT LAYER: Send response */
        send_via_tcp(client_socket, response, strlen(response));

        /* TRANSPORT LAYER: Close connection */
        printf("\n>>> Closing Connection (TCP FIN handshake)\n");
        close(client_socket);
        printf("    Socket closed, connection terminated\n");
    }

    close(server_socket);
    return 0;
}