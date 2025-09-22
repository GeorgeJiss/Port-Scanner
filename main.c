#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>
#include <fcntl.h>

#define DEFAULT_TIMEOUT 3
#define MAX_THREADS 100

// Global variables for signal handling
volatile int scan_interrupted = 0;

// Function prototypes
void print_usage(const char *program_name);
void signal_handler(int sig);
int set_socket_timeout(int sock, int timeout_seconds);
int scan_port(const char *hostname, struct sockaddr_in *target_addr, int port, int timeout);
void print_banner();

void print_banner() {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                     C TCP Port Scanner                       ║\n");
    printf("║              A Simple Network Diagnostic Tool               ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
}

void print_usage(const char *program_name) {
    printf("Usage: %s [OPTIONS] <hostname> <start_port> <end_port>\n", program_name);
    printf("\nOptions:\n");
    printf("  -t <seconds>    Connection timeout (default: 3 seconds)\n");
    printf("  -h              Show this help message\n");
    printf("\nExamples:\n");
    printf("  %s google.com 80 443\n", program_name);
    printf("  %s -t 5 github.com 20 25\n", program_name);
    printf("  %s localhost 3000 3010\n", program_name);
    printf("\n");
}

void signal_handler(int sig) {
    if (sig == SIGINT) {
        printf("\n\n[!] Scan interrupted by user. Cleaning up...\n");
        scan_interrupted = 1;
    }
}

int set_socket_timeout(int sock, int timeout_seconds) {
    struct timeval timeout;
    timeout.tv_sec = timeout_seconds;
    timeout.tv_usec = 0;
    
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        return -1;
    }
    
    if (setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0) {
        return -1;
    }
    
    return 0;
}

int scan_port(const char *hostname, struct sockaddr_in *target_addr, int port, int timeout) {
    int sock;
    int result;
    
    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return -1;
    }
    
    // Set socket timeout
    if (set_socket_timeout(sock, timeout) < 0) {
        close(sock);
        return -1;
    }
    
    // Set port in address structure
    target_addr->sin_port = htons(port);
    
    // Attempt connection
    result = connect(sock, (struct sockaddr *)target_addr, sizeof(*target_addr));
    
    close(sock);
    return result;
}

int main(int argc, char *argv[]) {
    char *hostname;
    int start_port, end_port;
    int timeout = DEFAULT_TIMEOUT;
    int opt;
    int open_ports = 0;
    int total_ports;
    
    struct hostent *host;
    struct sockaddr_in target_addr;
    char ip_str[INET_ADDRSTRLEN];
    
    // Set up signal handler for Ctrl+C
    signal(SIGINT, signal_handler);
    
    // Parse command line options
    while ((opt = getopt(argc, argv, "t:h")) != -1) {
        switch (opt) {
            case 't':
                timeout = atoi(optarg);
                if (timeout <= 0) {
                    fprintf(stderr, "Error: Timeout must be a positive integer\n");
                    exit(1);
                }
                break;
            case 'h':
                print_banner();
                print_usage(argv[0]);
                exit(0);
            default:
                print_usage(argv[0]);
                exit(1);
        }
    }
    
    // Check remaining arguments
    if (argc - optind < 3) {
        fprintf(stderr, "Error: Missing required arguments\n");
        print_usage(argv[0]);
        exit(1);
    }
    
    hostname = argv[optind];
    start_port = atoi(argv[optind + 1]);
    end_port = atoi(argv[optind + 2]);
    
    // Validate port range
    if (start_port < 1 || start_port > 65535 || end_port < 1 || end_port > 65535) {
        fprintf(stderr, "Error: Ports must be between 1 and 65535\n");
        exit(1);
    }
    
    if (start_port > end_port) {
        fprintf(stderr, "Error: Start port must be less than or equal to end port\n");
        exit(1);
    }
    
    print_banner();
    
    // Resolve hostname to IP address
    printf("[+] Resolving hostname '%s'...\n", hostname);
    host = gethostbyname(hostname);
    if (host == NULL) {
        fprintf(stderr, "[-] Error: Unable to resolve hostname '%s'\n", hostname);
        fprintf(stderr, "    %s\n", hstrerror(h_errno));
        exit(1);
    }
    
    // Prepare target address structure
    memset(&target_addr, 0, sizeof(target_addr));
    target_addr.sin_family = AF_INET;
    memcpy(&target_addr.sin_addr, host->h_addr_list[0], host->h_length);
    
    // Convert IP to string for display
    inet_ntop(AF_INET, &target_addr.sin_addr, ip_str, INET_ADDRSTRLEN);
    
    total_ports = end_port - start_port + 1;
    
    printf("[+] Target: %s (%s)\n", hostname, ip_str);
    printf("[+] Port range: %d-%d (%d ports)\n", start_port, end_port, total_ports);
    printf("[+] Timeout: %d seconds\n", timeout);
    printf("[+] Starting scan...\n\n");
    
    // Scan ports
    for (int port = start_port; port <= end_port && !scan_interrupted; port++) {
        int result = scan_port(hostname, &target_addr, port, timeout);
        
        if (result == 0) {
            printf("[OPEN]  Port %d/tcp\n", port);
            open_ports++;
        } else {
            // Uncomment the line below to show closed ports too
            // printf("[CLOSED] Port %d/tcp\n", port);
        }
        
        // Show progress for large scans
        if (total_ports > 50 && port % 20 == 0) {
            int progress = ((port - start_port + 1) * 100) / total_ports;
            printf("[INFO] Progress: %d%% (%d/%d ports scanned)\n", 
                   progress, port - start_port + 1, total_ports);
        }
    }
    
    // Print summary
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                         SCAN COMPLETE                        ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    
    if (scan_interrupted) {
        printf("[!] Scan was interrupted\n");
    }
    
    printf("[+] Scanned %d ports on %s (%s)\n", 
           total_ports, hostname, ip_str);
    printf("[+] Found %d open ports\n", open_ports);
    
    if (open_ports == 0) {
        printf("[-] No open ports found in the specified range\n");
    }
    
    printf("\n");
    return 0;
}