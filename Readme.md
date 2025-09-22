# C TCP Port Scanner

A robust command-line TCP port scanner written in C that demonstrates fundamental networking concepts and systems programming skills. This project showcases socket programming, TCP/IP networking, and system-level programming techniques essential for networking and embedded software engineering roles.

## Features

- **Fast TCP Port Scanning**: Efficient port scanning using raw sockets
- **Hostname Resolution**: Automatic DNS resolution from hostnames to IP addresses
- **Configurable Timeouts**: Customizable connection timeout settings
- **Signal Handling**: Graceful handling of interruptions (Ctrl+C)
- **Progress Tracking**: Real-time progress updates for large port ranges
- **Professional Output**: Clean, formatted output with scan summaries
- **Cross-Platform**: Compatible with Linux, macOS, and other Unix-like systems

## 🛠Technical Implementation

### Core Technologies
- **C Programming Language**: Low-level systems programming
- **Berkeley Sockets API**: TCP socket creation and connection handling
- **POSIX System Calls**: Signal handling, timeouts, and process management
- **DNS Resolution**: Hostname to IP address translation using `gethostbyname()`

### Key Concepts Demonstrated
- TCP/IP networking fundamentals
- Socket programming and connection management  
- Error handling and resource cleanup
- Signal handling for graceful termination
- Command-line argument parsing
- Memory management and buffer handling

## Requirements

- GCC compiler (or any C99-compatible compiler)
- POSIX-compliant operating system (Linux, macOS, BSD)
- Basic networking libraries (usually included with system)

## Installation & Build

### Quick Build
```bash
# Clone or download the project files
# Compile using make
make

# Or compile manually
gcc -Wall -Wextra -std=c99 -O2 -o port_scanner main.c
```

### Advanced Build Options
```bash
# Build debug version with debugging symbols
make debug

# Install system-wide (requires sudo)
make install

# Remove build files
make clean

# Run basic tests
make test

# Show all available targets
make help
```

## Usage

### Basic Syntax
```bash
./port_scanner [OPTIONS] <hostname> <start_port> <end_port>
```

### Command Options
- `-t <seconds>` : Set connection timeout (default: 3 seconds)
- `-h` : Display help message

### Examples

**Scan common web ports:**
```bash
./port_scanner google.com 80 443
```

**Scan with custom timeout:**
```bash
./port_scanner -t 5 github.com 20 25
```

**Scan local services:**
```bash
./port_scanner localhost 3000 3010
```

**Scan common service ports:**
```bash
./port_scanner example.com 20 80
```

### Sample Output
```
╔══════════════════════════════════════════════════════════════╗
║                     C TCP Port Scanner                       ║
║              A Simple Network Diagnostic Tool               ║
╚══════════════════════════════════════════════════════════════╝

[+] Resolving hostname 'google.com'...
[+] Target: google.com (142.250.191.14)
[+] Port range: 80-443 (364 ports)
[+] Timeout: 3 seconds
[+] Starting scan...

[OPEN]  Port 80/tcp
[OPEN]  Port 443/tcp

╔══════════════════════════════════════════════════════════════╗
║                         SCAN COMPLETE                        ║
╚══════════════════════════════════════════════════════════════╝
[+] Scanned 364 ports on google.com (142.250.191.14)
[+] Found 2 open ports
```

## Project Structure

```
port-scanner/
├── main.c          # Main source code with full implementation
├── Makefile        # Build configuration and automation
└── README.md       # Project documentation (this file)
```

## How It Works

1. **Hostname Resolution**: The program uses `gethostbyname()` to resolve the target hostname into an IP address.

2. **Socket Creation**: For each port in the specified range, a new TCP socket is created using `socket(AF_INET, SOCK_STREAM, 0)`.

3. **Connection Attempt**: The program attempts to establish a TCP connection to the target IP and port using `connect()`.

4. **Result Analysis**: 
   - Successful connection (return value 0) = **Open Port**
   - Failed connection (return value -1) = **Closed/Filtered Port**

5. **Resource Cleanup**: Each socket is properly closed after the connection attempt to prevent resource leaks.

## Security & Ethics

This tool is designed for:
- **Educational purposes**: Learning networking concepts
- **System administration**: Diagnosing network services  
- **Security testing**: Authorized penetration testing on owned systems

**Important**: Only use this tool on networks and systems you own or have explicit permission to test. Unauthorized port scanning may violate computer abuse laws and network policies.

## Potential Enhancements

- **Multi-threading**: Parallel port scanning for faster results
- **Service Detection**: Identify services running on open ports  
- **Output Formats**: XML, JSON, or CSV output options
- **IPv6 Support**: Extended support for IPv6 addresses
- **Stealth Scanning**: SYN scanning and other advanced techniques
- **Rate Limiting**: Configurable scanning speed controls

## Learning Outcomes

This project demonstrates proficiency in:

- **Systems Programming**: Low-level C programming with system calls
- **Network Programming**: TCP/IP sockets and connection handling
- **Error Handling**: Robust error checking and resource management
- **Tool Development**: Creating practical network diagnostic utilities
- **Code Organization**: Clean, maintainable, and well-documented code

## Troubleshooting

**Permission Issues**:
```bash
# Some systems may require elevated privileges
sudo ./port_scanner target.com 1 1024
```

**Compilation Errors**:
```bash
# Ensure you have GCC installed
sudo apt-get install build-essential  # Ubuntu/Debian
brew install gcc                       # macOS
```

**Network Issues**:
- Ensure target hostname is reachable
- Check firewall settings
- Verify network connectivity
