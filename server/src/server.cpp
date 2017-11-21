#include "server.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

using std::cout;
using std::endl;

// Create a server using the port number on which the server accepts
// connections.
Server::Server(int port_num): listen_socket_fd(socket(AF_INET, SOCK_STREAM, 0)) {
    if (listen_socket_fd == -1) {
        cout << "Failed to create client listening socket" << endl;
        return;
    }

    // Initialise the address of the server.
    memset(&server_addr, 0, sizeof(server_addr));

    // Set the code for the address family, this should always be set to AF_INET.
    server_addr.sin_family = AF_INET;
    // Set the port number, however it must first be converted to network byte
    // order, i.e. big endian, if it is not already.
    server_addr.sin_port = htons(port_num);
    // The IP address of the host. This is the address of the computer the
    // server is running on. The symbolic constant INADDR_ANY gets this address.
    server_addr.sin_addr.s_addr = INADDR_ANY;
}

// Sets-up a socket to listen for clients.
void Server::setup() {
    // Bind the socket to an address. This can fail, for example, if the socket
    // is already in use.
    int bind_result = bind(listen_socket_fd, (const struct sockaddr *)&server_addr, sizeof(server_addr));

    if (bind_result < 0) {
        cout << "Failed to bind client listening socket" << endl;
        return;
    }

    // Allow the server to listen on the socket for connections from clients.
    // The second argument is the backlog queue, i.e. the number of connections
    // that can be waiting while the process is handling a particular connection.
    // The maximum is 5.
    ::listen(listen_socket_fd, 5);
}

// Listen for messages from clients.
void Server::listen() {
    // Wait (block) for a client to connect. After this, communicating with the
    // client is done with the new socket file descriptor.
    sockaddr_in client_addr;
    // The size (in bytes) of the address of the client (this is required for
    // the accept system call)
    socklen_t client_addr_size_bytes;

    cout << "Waiting for client" << endl;

    int new_socket_fd = accept(listen_socket_fd, (struct sockaddr *) &client_addr, &client_addr_size_bytes);

    cout << "Client connected" << endl;

    // Read data from the client.
    size_t buffer_size = 256;
    char buffer[buffer_size];

    memset(buffer, 0, buffer_size);
    int bytes_read = read(new_socket_fd, buffer, buffer_size - 1);

    if (bytes_read == 0) {
        cout << "Connection Ended" << endl;
        return;
    }

    if (bytes_read < 0) {
        cout << "Error reading from socket" << endl;
        return;
    }

    cout << "Read: " << buffer << endl;
}
