#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

int main(void) {
    // The port number on which the server accepts connections.
    const int port_num = 1024;

    // Get the file descriptor for the socket. `socket` takes:
    //      1. The address domain of the socket (any two hosts on the  internet in this case)
    //      2. The type of socket. Either characters are read as a single stream (SOCK_STREAM)
    //         as if from a pipe or file. Or, a datagram socket, where message are read in chunks.
    //      3. The protocol. 0 indicates the OS should choose.
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd == -1) {
        cout << "Failed to create socket";
        return 0;
    }

    // Create and initialise the address of the server.
    sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));

    // Set the code for the address family, this should always be set to AF_INET.
    server_addr.sin_family = AF_INET;
    // Set the port number, however it must first be converted to network byte
    // order, i.e. big endian, if it is not already.
    server_addr.sin_port = htons(port_num);
    // The IP address of the host. This is the address of the computer the
    // server is running on. The symbolic constant INADDR_ANY gets this address.
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind a socket to an address. This can fail, for example, if the socket
    // is already in use.
    int bind_result = bind(socket_fd, (const struct sockaddr *)&server_addr, sizeof(server_addr));

    if (bind_result < 0) {
        cout << "Failed to bind" << endl;
        return 0;
    }

    // Allow the process to listen on the socket for connections.
    // The second argument is the backlog queue, i.e. the number of connections
    // that can be waiting while the process is handling a particular connection.
    // This is set to 0 since we only want 1 client.
    listen(socket_fd, 0);

    // Wait (block) for a client to connect. After this, communicating with the
    // client is done with the new socket file descriptor.
    sockaddr_in client_addr;
    // The size (in bytes) of the address of the client (this is required for
    // the accept system call)
    socklen_t client_addr_size_bytes;

    cout << "Waiting for client" << endl;

    int new_socket_fd = accept(socket_fd, (struct sockaddr *) &client_addr, &client_addr_size_bytes);

    cout << "Client connected" << endl;

    // Read data from the client.
    size_t buffer_size = 256;
    char buffer[buffer_size];

    while (1) {
        memset(buffer, 0, buffer_size);
        int bytes_read = read(new_socket_fd, buffer, buffer_size - 1);

        if (bytes_read == 0) {
            cout << "Connection Ended" << endl;
            return 0;
        }

        if (bytes_read < 0) {
            cout << "Error reading from socket" << endl;
            return 0;
        }

        cout << "Read: " << buffer << endl;
    }

    return 0;
}
