/* 
 * TCP/IP Protocol Demonstration - CLIENT
 * This demonstrates TCP/IP protocol layers in action
 * 
 * TCP/IP Model Layers:
 * - Application Layer: Message preparation
 * - Transport Layer: TCP socket operations
 * - Internet Layer: IP address and routing
 * - Network Access Layer: MAC addressing & physical transmission (hardware)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9999
#define BUFFER_SIZE 256

/* Application Layer: Message preparation */
void prepare_message(const char *user_input, char *buffer, size_t size) {
    printf("\n=== APPLICATION LAYER ===\n");
    printf("Preparing message for transmission:\n");
    printf("  User Input: '%s'\n", user_input);
    printf("  Message Length: %zu bytes\n", strlen(user_input));
    
    strncpy(buffer, user_input, size - 1);
    buffer[size - 1] = '\0';
    printf("  Encoded: READY FOR TRANSPORT LAYER\n");
}

/* Network Layer: Show IP address handling */
void show_network_layer(const char *server_ip, uint16_t port) {
    printf("\n=== NETWORK LAYER (IP) ===\n");
    printf("IP Address Resolution:\n");
    printf("  Server IP: %s\n", server_ip);
    printf("  Server Port: %d\n", port);
    printf("  Protocol: TCP (IPv4)\n");
    printf("  Address Family: AF_INET\n");
    printf("  Byte Order Conversion: htons() for network byte order\n");
}

/* Data Link Layer: Show MAC (simulated) */
void show_datalink_layer() {
    printf("\n=== NETWORK ACCESS LAYER ===\n");
    printf("Frame Information (MAC/Physical):\n");
    printf("  Source MAC: (client system MAC)\n");
    printf("  Destination MAC: (server system MAC)\n");
    printf("  ARP Protocol: used to resolve IP to MAC\n");
    printf("  Hardware handles: frame construction and physical transmission\n");
}

/* Transport Layer: TCP connection with 3-way handshake */
void establish_tcp_connection(int *socket_fd, struct sockaddr_in *addr) {
    printf("\n>>> Creating TCP Socket (Transport Layer)\n");
    *socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (*socket_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    printf("    Socket created: FD=%d, Protocol=TCP/IPv4\n", *socket_fd);

    printf("\n>>> Initiating TCP 3-Way Handshake (Transport Layer)\n");
    printf("    Step 1: SYN - Client sends SYN packet to server\n");
    printf("    Step 2: SYN-ACK - Server responds with SYN-ACK\n");
    printf("    Step 3: ACK - Client sends ACK, connection established\n");
    
    if (connect(*socket_fd, (struct sockaddr *)addr, sizeof(*addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }
    printf("    ✓ TCP Connection ESTABLISHED\n");
}

/* Transport Layer: Send data via TCP */
void send_via_tcp(int socket_fd, const char *data) {
    printf("\n>>> Sending Data (Transport Layer - TCP)\n");
    printf("    TCP Segment Details:\n");
    printf("      Source Port: (ephemeral, kernel assigned)\n");
    printf("      Destination Port: %d\n", SERVER_PORT);
    printf("      Data Length: %zu bytes\n", strlen(data));
    printf("      TCP Flags: PSH (push), ACK\n");
    printf("      Sequence Number: (kernel managed)\n");
    printf("      Acknowledgment Number: (kernel managed)\n");
    printf("      Checksum: (kernel calculated)\n");
    
    ssize_t sent = send(socket_fd, data, strlen(data), 0);
    if (sent > 0) {
        printf("      ✓ Sent: %zd bytes\n", sent);
        printf("    TCP guarantees: in-order delivery, error checking, retransmission\n");
    }
}

/* Transport Layer: Receive data via TCP */
void receive_via_tcp(int socket_fd, char *buffer, size_t size) {
    printf("\n>>> Receiving Response (Transport Layer - TCP)\n");
    printf("    Waiting for TCP segment from server...\n");
    
    ssize_t received = recv(socket_fd, buffer, size - 1, 0);
    if (received > 0) {
        buffer[received] = '\0';
        printf("    ✓ Received: %zd bytes\n", received);
        printf("    TCP handled: sequencing, checksums, flow control\n");
    }
}

/* Application Layer: Display received response */
void display_response(const char *buffer) {
    printf("\n=== APPLICATION LAYER (Response) ===\n");
    printf("Server Response:\n");
    printf("  Message: '%s'\n", buffer);
    printf("  Length: %zu bytes\n", strlen(buffer));
}

int main(int argc, char *argv[]) {
    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║      TCP/IP Protocol Demonstration - CLIENT            ║\n");
    printf("║      Research Project on TCP/IP Layers                 ║\n");
    printf("╚════════════════════════════════════════════════════════╝\n");

    /* Get message from command line or use default */
    const char *message = (argc > 1) ? argv[1] : "Hello from TCP Client!";

    /* APPLICATION LAYER: Prepare message */
    char app_buffer[BUFFER_SIZE];
    prepare_message(message, app_buffer, BUFFER_SIZE);

    /* NETWORK LAYER: Setup address */
    show_network_layer(SERVER_IP, SERVER_PORT);
    
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    /* DATA LINK LAYER: Info */
    show_datalink_layer();

    /* TRANSPORT LAYER: Establish connection */
    int socket_fd;
    establish_tcp_connection(&socket_fd, &server_addr);

    /* TRANSPORT LAYER: Send data */
    send_via_tcp(socket_fd, app_buffer);

    /* TRANSPORT LAYER: Receive response */
    char response[BUFFER_SIZE];
    memset(response, 0, BUFFER_SIZE);
    receive_via_tcp(socket_fd, response, BUFFER_SIZE);

    /* APPLICATION LAYER: Display response */
    display_response(response);

    /* TRANSPORT LAYER: Close connection */
    printf("\n>>> Closing Connection (TCP FIN handshake)\n");
    printf("    Step 1: FIN - Client sends FIN packet\n");
    printf("    Step 2: ACK - Server acknowledges\n");
    printf("    Step 3: FIN - Server sends FIN\n");
    printf("    Step 4: ACK - Client acknowledges\n");
    printf("    ✓ Connection CLOSED\n");
    
    close(socket_fd);

    printf("\n╔════════════════════════════════════════════════════════╗\n");
    printf("║ Protocol Demonstration Complete                        ║\n");
    printf("╚════════════════════════════════════════════════════════╝\n\n");

    return 0;
}